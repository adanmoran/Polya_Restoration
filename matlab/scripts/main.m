%% Reset workspace
clear
close all
clc

%% Compile C File for Lee Filter
mex computelee.c

%% Load Image in Grayscale or Colour
% imagepath = '../images/lena512.bmp';
% imagepath = '../images/oil_spill.jpg';
% imagepath = '../images/aerial1.tiff';
% imagepath = '../images/pentagon.tiff';
imagepath = '../images/goldengate.tiff';

image = imread(imagepath);

%% Preferences
im_info = imfinfo(imagepath);
switch(im_info.ColorType)
    case('truecolor')
        prefs.image.type = 'rgb';
    case('grayscale')
        prefs.image.type = 'gray';
    case('indexed')
        prefs.image.type = 'bw';
    otherwise
        disp("Can't detect image type. Need to override prefs.image.type");
end
% prefs.image.type = 'rgb'; % 'bw', 'gray', 'rgb', 'ycbcr'

% Lena threshold: 0.52
% oil_spill threshold: 0.18
prefs.image.binarize_thresh = 0.52;

prefs.edges.use_edges = false;
prefs.edges.filter = 'canny';
% The variance of Canny's gaussian filter. Default is sqrt(2)
prefs.edges.sigma = 3; 
% Threshold for the Canny method
% Oil Spill: 0.4
% Lena: with gauss = 0.2, with burst = 0.38
% Two ships: 0.5
prefs.edges.thresh = 0.2;

prefs.quant.num_ball_types = 30; % [2 - 256]
prefs.quant.type = 'unif'; % unif, lloyd
prefs.quant.inverse = 'mid'; % low, high, mid

prefs.adj.radius = 3;
prefs.adj.norm = 2;

prefs.polya.sample_type = 'median'; % 'median', 'random'
prefs.polya.starting_balls = 100; % Starting balls in each urn
% Number of balls to add to the urn after each polya step
prefs.polya.balls_to_add = 60;
prefs.polya.iterations = 8;

prefs.median.iterations = 8;

prefs.lee.kernel = [5 5]; % window size for lee filter
prefs.lee.iterations = 8;

% Noise parameters
% BW Gaussian Noise Parameters
noise.bw.gaussian_std_dev = 1;
noise.bw.gaussian_mean = 0;
noise.bw.gaussian_confidence_interval = 0.8; % Error rate is 1 - this

% Colour & Greyscale Noise Parameters
noise.gaussian.sigma = 0.01;
noise.gaussian.mean = 0;

noise.bursty.type = 'binary'; % 'gaussian' or 'binary'
noise.bursty.transition_prob = 0.98;
noise.bursty.error = 0.1; % 0.2 for gaussian, 0.1 for binary
noise.bursty.mean = 0;
noise.bursty.sigma = 100;

noise.gauss_markov.correlation = 0.9; % (-1, 1)
noise.gauss_markov.mean = 0;
noise.gauss_markov.sigma = 10;

noise.type = 'gauss-markov'; % 'gaussian' or 'burst' or 'both' or 'none' 
                     % or 'gauss-markov'


%% Add Gaussian or Bursty Noise
rng(0, 'twister');

if strcmp(prefs.image.type, 'bw')
    % If it's an rgb image, convert it to grayscale
    if size(size(image), 2) > 2
        image = rgb2gray(image);
    end
    image = imbinarize(image, prefs.image.binarize_thresh);
end

noisy_image = add_noise(image, prefs, noise);

figure;
imshowpair(image, noisy_image, 'montage');

% Initialize the image for filter comparisons
medianed = noisy_image;
lee = noisy_image;

%% Run Colour or Greyscale Polya Filter
if strcmp(prefs.image.type, 'rgb') || strcmp(prefs.image.type, 'ycbcr')
    if strcmp(prefs.image.type, 'ycbcr')
        noisy_image = rgb2ycbcr(noisy_image);
    end
    % Save each colour channel for polya and median filter
    channel_1 = noisy_image(:, :, 1);
    channel_2 = noisy_image(:, :, 2);
    channel_3 = noisy_image(:, :, 3);
    [med_1, med_2, med_3] = deal(channel_1, channel_2, channel_3);
    [lee_1, lee_2, lee_3] = deal(channel_1, channel_2, channel_3);
    
    % Run the greyscale polya filter on each channel individually
    channel_1_p = polyafilt(channel_1, prefs);
    channel_2_p = polyafilt(channel_2, prefs);
    channel_3_p = polyafilt(channel_3, prefs);
    
    % Run the median filter on each channel individually
    for i = 1:prefs.median.iterations
        med_1 = medfilt2(med_1);
        med_2 = medfilt2(med_2);
        med_3 = medfilt2(med_3);
    end
    
    % Run the Lee filter on each channel individually
    for i = 1:prefs.lee.iterations
        lee_1 = leefilt(channel_1, lee_1, prefs.lee.kernel);
        lee_2 = leefilt(channel_2, lee_2, prefs.lee.kernel);
        lee_3 = leefilt(channel_3, lee_3, prefs.lee.kernel);
    end
    
    % Reconstruct the colour image
    output = cat(3, channel_1_p, channel_2_p, channel_3_p);
    medianed = cat(3, med_1, med_2, med_3);
    lee = cat(3, lee_1, lee_2, lee_3);
    
    if strcmp(prefs.image.type, 'ycbcr')
        output = ycbcr2rgb(output);
        medianed = ycbcr2rgb(medianed);
        lee = ycbcr2rgb(lee);
        noisy_image = ycbcr2rgb(noisy_image);
    end
        
elseif strcmp(prefs.image.type, 'gray')
    % Run the grayscale polya filter
    output = polyafilt(noisy_image, prefs);
    for i = 1:prefs.median.iterations
        medianed = medfilt2(medianed);
    end
    for i = 1:prefs.lee.iterations
        lee = leefilt(image, lee, prefs.lee.kernel);
    end        
    
elseif strcmp(prefs.image.type, 'bw')
    prefs.quant.num_ball_types = 3;
    
    % Run the black and white polya filter
    output = polyafilt(noisy_image, prefs);
    for i = 1:prefs.median.iterations
        medianed = medfilt2(medianed);
    end
    for i = 1:prefs.lee.iterations
        lee = leefilt(image, lee, prefs.lee.kernel);
    end
    
    % Convert to numeric type for metric functions
    output = uint8(output);
    image = uint8(image);
    medianed = uint8(medianed);
    lee = uint8(lee);
end

%% Display Results
figure;
imshowpair(noisy_image, output, 'montage');
title ('Noise vs Polya');
fprintf('Polya vs Original MSE: %.3f\n', immse(output, image));
fprintf(' ---- ssim: %.3f\n', ssim(output, image));
fprintf(' ---- psnr: %.3f\n', psnr(output, image));

figure;
imshowpair(noisy_image, medianed, 'montage');
title('Noise vs Median');
fprintf('Median vs Original MSE: %.3f\n', immse(medianed, image));
fprintf(' ---- ssim: %.3f\n', ssim(medianed, image));
fprintf(' ---- psnr: %.3f\n', psnr(medianed, image));

figure;
imshowpair(noisy_image, lee, 'montage');
title('Noise vs Lee');
fprintf('Lee vs Original MSE: %.3f\n', immse(lee, image));
fprintf(' ---- ssim: %.3f\n', ssim(lee, image));
fprintf(' ---- psnr: %.3f\n', psnr(lee, image));

figure;
imshowpair(output, medianed, 'montage');
title('Polya vs Median');

figure;
imshowpair(output, lee, 'montage');
title('Polya vs Lee');