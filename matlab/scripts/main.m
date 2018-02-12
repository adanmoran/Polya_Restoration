%% Reset workspace
clear
close all
clc
%% Load Image in Grayscale or Colour
% image = imread('../images/lena512.bmp');
% image = rgb2gray(imread('../images/oil_spill.jpg'));
% image = imread('../images/aerial1.tiff');
% image = imread('../images/pentagon.tiff');
image = imread('../images/goldengate.tiff');

%% Preferences
prefs.image.type = 'rgb'; % 'bw', 'grey', 'rgb', 'ycbcr'

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

noise.type = 'both'; % 'gaussian' or 'burst' or 'both' or 'none'


%% Add Gaussian or Bursty Noise
rng(0, 'twister');

if strcmp(prefs.image.type, 'bw')
    % If it's an rgb image, convert it to grayscale
    if size(size(image),2) > 2
        image = rgb2gray(image);
    end
    image = imbinarize(image, prefs.image.binarize_thresh);
end

noisy_image = add_noise(image, prefs, noise);

figure;
imshowpair(image, noisy_image, 'montage');

% Initialize the image for the median filter comparison
medianed = noisy_image;

%% Run Colour or Greyscale Polya Filter
if strcmp(prefs.image.type, 'rgb') || strcmp(prefs.image.type, 'ycbcr')
    if strcmp(prefs.image.type, 'ycbcr')
        noisy_image = rgb2ycbcr(noisy_image);
    end
    channel_1 = noisy_image(:, :, 1);
    channel_2 = noisy_image(:, :, 2);
    channel_3 = noisy_image(:, :, 3);
    
    % Run the greyscale polya filter on each channel individually
    channel_1_p = polyafilt(channel_1, prefs);
    channel_2_p = polyafilt(channel_2, prefs);
    channel_3_p = polyafilt(channel_3, prefs);
    
    % Reconstruct the RGB image
    output = cat(3, channel_1_p, channel_2_p, channel_3_p);

    if strcmp(prefs.image.type, 'ycbcr')
        output = ycbcr2rgb(output);
        noisy_image = ycbcr2rgb(noisy_image);
    end
    
    for i = 1:prefs.median.iterations
        medianed = medfilt3(medianed);
    end
    
elseif strcmp(prefs.image.type, 'grey')
    % Run the greyscale polya filter
    output = polyafilt(noisy_image, prefs);
    for i = 1:prefs.median.iterations
        medianed = medfilt2(medianed);
    end
    
elseif strcmp(prefs.image.type, 'bw')
    prefs.quant.num_ball_types = 3;
    
    % Run the black and white polya filter
    output = polyafilt(noisy_image, prefs);
    for i = 1:prefs.median.iterations
        medianed = medfilt2(medianed);
    end
    
    % Convert to numeric type for metric functions
    output = uint8(output);
    image = uint8(image);
    medianed = uint8(medianed);
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
imshowpair(output, medianed, 'montage');
title('Polya vs Median');