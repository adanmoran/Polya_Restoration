%% Reset workspace
clear
close all
clc

%% Compile C File for Lee Filter
fprintf('Mexing Lee Filter\n');
mex computelee.c

%% Compile C++ File for Polya_Eigen Function
fprintf('Mexing Polya Eigen Function\n');
ipath = ['-I' '../../c_plus_plus/include/'];
ipath2 = ['-I' '../../c_plus_plus/external/eigen/'];
polyapath = '../../c_plus_plus/src/polya/polya.cpp';
mex(ipath, ipath2, "polya_eigen.cpp", polyapath)

%% Pick an image for testing
fprintf('Loading image\n');
imagepath = '../images/lena512.bmp';
[~,imagename,ext] = fileparts(imagepath);
% imagepath = '../images/oil_spill.jpg';
% imagepath = '../images/aerial1.tiff';
% imagepath = '../images/pentagon.tiff';
% imagepath = '../images/goldengate.tiff';

image = imread(imagepath);

%% Assign the values over which we will optimize
% Don't need to optimize starting balls; this just gives us the
% "resolution" of the ballsToAdd variable
startingBalls = 100;

% Quantization bins will be optimized over these values. They are powers of
% 2 because 256 will divide nicely into each of these.
binsToOptimize = [32 64 128 256];

% ballsToAdd will be optimized over these values
deltaToOptimize = [20 40 60 80 100 120 140 160 180 200];

% radius will be optimized from 1 : maxRadius in the 2-norm.
% Note that if the edge map is used, the max radius will be 2 no matter
% what is chosen here.
maxRadius = 3;

% N will be optimized in the range  1 : maxIterations. The iterations will
% stop when the improvement in MSE is less than epsilon, even if they
% haven't reached maxIterations.
% i.e. 
% for N = 1 : maxIterations
%    mse = <calculate error from polyaFilt for N iterations>
%    if abs(mse - previous_mse) < epsilon
%        break
%    end
% end
epsilon = 0.5;
maxIterations = 50;

%% Preferences that do not need to be optimized
im_info = imfinfo(imagepath);
switch(im_info.ColorType)
    case('truecolor')
        prefs.image.type = 'rgb';
    case('grayscale')
    case('indexed')
        prefs.image.type = 'gray';
    otherwise
        error("Can't detect image type. Need to override prefs.image.type");
end

% We do not binarize for optimal tests, as binarization is non-optimal. See
% main.m for the preferences options if you want to change this.

% Edge map optimization must use edge map sigma/thresh retrieved from the
% GUI.
prefs.edges.use_edges = false;
prefs.edges.filter = 'canny';
% The variance of Canny's gaussian filter. Default is sqrt(2)
prefs.edges.sigma = 3; 
% Threshold for the Canny method
prefs.edges.thresh = 0.38;

prefs.polya.sample_type = 'median'; % 'median', 'random'

prefs.quant.type = 'lloyd'; % unif, lloyd
prefs.quant.inverse = 'mid'; % low, high, mid
prefs.adj.norm = 2;

%% Structure to store optimal data and noise

norm = prefs.adj.norm;
quant = prefs.quant;

noises = 1:3 ;
files = -1 * ones(size(noises));
logs = -1 * ones(size(noises));
noise = generate_noise(2);
noise.name
parfor i = noises
% Generate a noise for this image and this thread
noise = generate_noise(i);

% Open a file for this noise type
fname = sprintf('%s_%s_optimal', imagename, noise.name);
files(i) = fopen(sprintf('./optimal_log/%s.csv', fname),'wt');
logs(i) = fopen(sprintf('./optimal_log/%s.log', fname), 'wt');

fprintf(files(i), 'Radius, # Bins, Delta, N, MSE, PSNR, SSIM\n');
fprintf(logs(i),'Log Started on %s\n\n', date);
                     
% % Information about image, to add to CSV at the end
% optimalResults.image.name = imagepath;
% optimalResults.image.noise = noise;
% optimalResults.image.image = image;
% 
% % Optimized preferences will be stored here
% optimalResults.prefs.radius = 1;
% optimalResults.prefs.norm = norm; 
% 
% optimalResults.prefs.num_ball_types = 256;
% optimalResults.prefs.balls_to_add = startingBalls;
% 
% optimalResults.prefs.quant = quant;
% 
% optimalResults.prefs.iterations = 0;
% optimalResults.prefs.epsilon = epsilon;
% 
% % Optimized results on metrics
% optimalResults.polya.mse = 255*255;
% optimalResults.polya.psnr = Inf;
% optimalResults.polya.ssim = 0;
% optimalResults.polya.filtered = zeros(size(image));
% 
% optimalResults.median.mse = 255*255;
% optimalResults.median.psnr = Inf;
% optimalResults.median.ssim = 0;
% optimalResults.median.filtered = zeros(size(image));
% 
% optimalResults.lee.mse = 255*255;
% optimalResults.lee.psnr = Inf;
% optimalResults.lee.ssim = 0;
% optimalResults.lee.filtered = zeros(size(image));
optimalMse = 255 * 255;

%% Run the optimization for this image and noise type
rng(0, 'twister');

fprintf(logs(i), 'Adding Noise\n');
noisy_image = add_noise(image, prefs, noise);

%figure;
%imshowpair(image, noisy_image, 'montage');

% Initialize the image for filter comparisons
medianed = noisy_image;
lee = noisy_image;

% Optimize radius
fprintf(logs(i), 'Optimizing for %s\n', fname);
for r = 1:maxRadius
    adjacency = get_sparse_adj(size(noisy_image), ...
                               r, ...
                               prefs.adj.norm);
    % Optimize Quantization bins
    for numBins = binsToOptimize
        [quantized_image, partition, codebook] = quantize_image(...
                                            noisy_image, ... 
                                            numBins, ... 
                                            prefs.quant.type);
        useCpp = numBins > 64;
        
        % Optimize Delta
        for delta = deltaToOptimize
            % Initialize urns
            fprintf(logs(i), 'Running polya filter for radius = %d, bins = %d, delta = %d.\n',...
                r, numBins, delta);

            if useCpp
                Delta = delta * speye(numBins);
            else
                Delta = delta * eye(numBins);
            end
            urns = initialize_polya_urns(quantized_image, ...
                                         startingBalls, ...
                                         numBins);
            % Run polya filter
            
            previousMse = immse(noisy_image, image);
            outputImage = noisy_image;
            previuosOutput = outputImage;
            
            for N = 1 : maxIterations
                tic
                fprintf(logs(i), 'Iteration %d of %d | Duration = ',...
                    N, maxIterations);
                % Generate new urn
                if useCpp
                    urns = polya_eigen(urns, adjacency, Delta, prefs.polya.sample_type, 1);
                else
                    urns = polya(urns, adjacency, Delta, prefs.polya.sample_type);
                end            
                % Convert back to image
                outputImage = image_from_urns(size(noisy_image), urns);
                outputImage = inverse_quantize_image(outputImage, ...
                                    prefs.quant.inverse, ...
                                    partition, ...
                                    codebook);
                % Compute MSE and stop if we are not improving
                newMse = immse(outputImage, image);
                fprintf(logs(i), '%.3f | MSE = %.4f\n', toc, newMse);
                % Stop only when we are not improving as much as desired
                % (given we've run it twice at least
                if floor(newMse) > floor(previousMse)
                    % If we went up this time, the last iteration is more optimal
                    N = N - 1;
                    outputImage = previousOutput;
                    newMse = previousMse
                    break
                elseif abs(previousMse - newMse) < epsilon && N < 2
                    break;
                end
                previousMse = newMse;
                previousOutput = outputImage;
            end
%             if newMse < optimalResults.polya.mse
%                 % If this MSE is better than the optimal, these preferences
%                 % are optimal
%                 optimalResults.polya.mse = newMse;
%                 optimalResults.polya.psnr = psnr(outputImage, image);
%                 optimalResults.polya.ssim = ssim(outputImage, image);
%                 optimalResults.polya.filtered = outputImage;
% 
%                 optimalResults.prefs.radius = r;
%                 optimalResults.prefs.num_ball_types = numBins;
%                 optimalResults.prefs.balls_to_add = delta;
%                 optimalResults.prefs.iterations = N;
%                 fprintf('New optimal MSE: %.2f\n', newMse);
%             end
            if newMse < optimalMse
                fprintf(logs(i), 'New optimal MSE: %.2f\n', newMse);
                % Save the results to a CSV file
                fprintf(files(i), '%d, %d, %d, %d, %.3f, %.3f, %.3f\n', ...
                    r, numBins, delta, N, ...
                    newMse, psnr(outputImage, image), ssim(outputImage, image));
                % Save the most recent output image
                imwrite(outputImage, sprintf('./frames/%s.%s',fname, ext));
                % Keep track of the optimal MSE
                optimalMse = newMse;
            end
        end
    end
end
fclose(files(i));
fclose(logs(i));
% Now run the median for this noise type and compute the optimal results
end

% Ensure the files are closed
for file = files
    if file ~= -1
        fclose(file);
    end
end
% Ensure the logs are closed
for log = logs
    if log ~= -1
        fclose(log);
    end
end

%% Generate noise based on loop variable (for parfor)
function noise = generate_noise(i)
    noise = struct;
    switch(i)
        case 1 % Low gauss-markov noise
            noise.name = 'gauss_markov_low';
            noise.gauss_markov.correlation = 0.9; % (-1, 1)
            noise.gauss_markov.mean = 0;
            noise.gauss_markov.sigma = 1;
            noise.type = 'gauss-markov';
            return;
%         case 2 % Medium gauss-markov noise
%         case 3 % High gauss-markov noise
        case {2,3,4} % Low gaussian noise with binary erasure
            noise.name = 'gaussian_and_binary_erasure';
            noise.gaussian.sigma = 0.01;
            noise.gaussian.mean = 0;

            noise.bursty.type = 'binary';
            noise.bursty.transition_prob = 0.98;
            noise.bursty.error = 0.1;
            noise.type = 'both';
            return;
        case 5 % Medium gaussian noise with binary erasure
        case 6 % High gaussian noise with binary erasure
        case 7 % Hidden-markov noise
            noise.name = 'hidden_markov_0.01_100';
            noise.gaussian.sigma = 0.01;
            noise.gaussian.mean = 0;

            noise.bursty.type = 'gaussian';
            noise.bursty.transition_prob = 0.98;
            noise.bursty.error = 0.2;
            noise.bursty.mean = 0;
            noise.bursty.sigma = 100;
            noise.type = 'both';
            return;
    end
end