clear all
close all
clc
%% Load Lena in Grayscale
lena = imread('..\images\lena512.bmp');

%% Add Gaussian Noise
rng(0, 'twister');

% Gaussian noise values
sigma = 0.02;
mean = 0;
noisy_lena = imnoise(lena, 'gaussian', mean, sigma);
figure, imshowpair(lena, noisy_lena, 'montage');

%% Take the Edge Map
% The variance of Canny's gaussian filter. Default is sqrt(2)
sigma = 3;
% Threshold for the Canny method
thresh = 0.2;
edges = edge(noisy_lena, 'canny', thresh, sigma);
figure;
imshowpair(noisy_lena, edges, 'montage');

%% Build Adjacency Matrix
adj_radius = 1;
adj_norm = 1;

% Get image initial adjacency matrix
adjacency = get_sparse_adj(size(lena), adj_radius, adj_norm);
% Remove adjacency connections based on edge map
adjacency = adjacency_minus_edge_d(adjacency, edges, adj_radius);

%% Setup Polya Model
starting_balls = 10;
% Initialize urns with standard adjacency matrix
urns = initialize_polya_urns(noisy_lena, adjacency, starting_balls);

%% Iterate the polya model
N = 10;

for i = 1:N
    %TODO: create the polya model for grayscale
    % urns = polya(urns, adjacency, Delta, sample_type);
end

%% Build the final image
output = image_from_urns(size(noisy_lena), urns);
