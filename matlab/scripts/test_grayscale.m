clear
close all
clc
%% Load Lena in Grayscale
lena = rgb2gray(imread('../images/oil_spill.jpg'));
%lena(343:500, :) = lena(343);
%lena = imread('..\images\lena512.bmp');

%% Quantize Image
numBallTypes = 256;
lena = quantize_image(lena, numBallTypes);

%% Add Gaussian Noise
rng(0, 'twister');

% Gaussian noise values
sigma = 0.01;
mean = 0;
noisy_lena = lena; %imnoise(lena, 'gaussian', mean, sigma);
figure, imshowpair(lena, noisy_lena, 'montage');

%% Take the Edge Map
% The variance of Canny's gaussian filter. Default is sqrt(2)
sigma = 3;
% Threshold for the Canny method
% Oil Spill: 0.4
% Lena: 0.2
% Two ships: 0.5
thresh = 0.4;
edges = edge(noisy_lena, 'canny', thresh, sigma);
figure;
imshowpair(noisy_lena, edges, 'montage');

%% Build Adjacency Matrix
adj_radius = 2;
adj_norm = 1;

% Get image initial adjacency matrix
adjacency = get_sparse_adj(size(lena), adj_radius, adj_norm);

%% Setup Polya Model
% Sample Type (random, median)
sample_type = 'median';
%Starting balls in each urn
starting_balls = 100;
% Number of balls to add to the urn after each polya step
balls_to_add = 75;
% Matrix of ball addition
Delta = balls_to_add * eye(numBallTypes);
% Initialize urns with standard adjacency matrix
%urns = initialize_polya_urns(noisy_lena, adjacency, starting_balls);
urns = initialize_polya_urns(noisy_lena, starting_balls, numBallTypes);

% Initialize the image for the median filter comparison
medianed = noisy_lena;

%% Iterate the polya model
N = 6;

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
output = uint8(image_from_urns(size(noisy_lena), urns));
toc
figure;
imshowpair(noisy_lena, output, 'montage');
title ('Noise vs Polya');
fprintf('Polya vs Original MSE: %.3f\n', immse(output, lena));
fprintf(' ---- ssim: %.3f\n', ssim(output, lena));
figure;
imshowpair(noisy_lena, medianed, 'montage');
title('Noise vs Median');
fprintf('Median vs Original MSE: %.3f\n', immse(medianed, lena));
fprintf(' ---- ssim: %.3f\n', ssim(medianed, lena));
figure;
imshowpair(output, medianed, 'montage');
title('Polya vs Median');