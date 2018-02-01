clear
close all
clc
%% Load Image in Grayscale
%image = imread('../images/lena512.bmp');
% image = rgb2gray(imread('../images/oil_spill.jpg'));
%image = imread('../images/aerial1.tiff');
image = imread('../images/pentagon.tiff');

%% Add Gaussian Noise
rng(0, 'twister');

% Gaussian noise values
sigma = 0.01;
mean = 0;

% Bursty noise values
burstType = 'binary'; % 'gaussian' or 'binary'
correlation = 0.98;
error = 0.1; % 0.2 for gaussian, 0.1 for binary
burstMean = 0;
burstSigma = 100;
% Noise type
% 'gaussian' or 'burst' or 'both'
noise = 'both';

noisy_image = image;

if strcmp(noise, 'gaussian')
    noisy_image = imnoise(image, 'gaussian', mean, sigma);
elseif strcmp(noise, 'burst')
    if strcmp(burstType,'binary')
        noisy_image = add_bursty_noise(image, correlation, error, 'binary');
    else
        noisy_image = add_bursty_noise(image, correlation, error, 'gaussian', burstMean, burstSigma);
    end
elseif strcmp(noise, 'both')
    noisy_image = imnoise(image, 'gaussian', mean, sigma);
    if strcmp(burstType,'binary')
        noisy_image = add_bursty_noise(noisy_image, correlation, error, 'binary');
    else
        noisy_image = add_bursty_noise(noisy_image, correlation, error, 'gaussian', burstMean, burstSigma);
    end
end

figure, imshowpair(image, noisy_image, 'montage');

% Initialize the image for the median filter comparison
medianed = noisy_image;

%% Take the Edge Map
% The variance of Canny's gaussian filter. Default is sqrt(2)
sigma = 3;
% Threshold for the Canny method
% Oil Spill: 0.4
% Lena: with gauss = 0.2, with burst = 0.38
% Two ships: 0.5

thresh = 0.2;
edges = edge(noisy_image, 'canny', thresh, sigma);

use_edges = false;

if use_edges
    figure;
    imshowpair(noisy_image, edges, 'montage');
end


%% Quantization Parameters
numBallTypes = 30; % [2 - 256]
quantization = 'norm'; % unif, norm, exp
inverse_quantization = 'high'; % low, high, mid

[noisy_image, partition, codebook] = quantize_image(noisy_image, numBallTypes, quantization);

%% Build Adjacency Matrix
adj_radius = 3;
adj_norm = 2;

% Get image initial adjacency matrix
adjacency = get_sparse_adj(size(image), adj_radius, adj_norm);

%% Setup Polya Model
% Sample Type (random, median)
sample_type = 'median';
%Starting balls in each urn
starting_balls = 100;
% Number of balls to add to the urn after each polya step
balls_to_add = 60;
% Matrix of ball addition
Delta = balls_to_add * eye(numBallTypes);
% Initialize urns with standard adjacency matrix
urns = initialize_polya_urns(noisy_image, starting_balls, numBallTypes);

% Remove adjacency connections based on edge graph
%adjacency = remove_edge_connections(adjacency, edges);
if use_edges
    adjacency = adjacency_minus_edge_d(adjacency, edges, adj_radius);
end
%% Iterate the polya model
N = 8;

for i = 1:N
    tic
    %TODO: create the polya model for grayscale
    fprintf('Iteration %d of %d | Duration: ',i,N);
    urns = polya(urns, adjacency, Delta, sample_type);
    fprintf('%.3f\n', toc);
    medianed = medfilt2(medianed);
end

%% Build the final image
tic
output = image_from_urns(size(noisy_image), urns);
output = inverse_quantize_image(output, inverse_quantization, partition, codebook);
toc
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