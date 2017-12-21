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
noisy = imnoise(lena, 'gaussian', mean, sigma);
figure, imshowpair(lena, noisy, 'montage');

%% Take the Edge Map
% The variance of Canny's gaussian filter. Default is sqrt(2)
sigma = 3;
% Threshold for the Canny method
thresh = 0.2;
edges = edge(noisy, 'canny', thresh, sigma);
figure;
imshowpair(noisy, edges, 'montage');

%% Build Adjacency Matrix
adj_radius = 1;
adj_norm = 1;

% Get image initial adjacency matrix
adjacency = get_sparse_adj(size(lena), adj_radius, adj_norm);

%% Setup Polya Model
starting_balls = 10;
% Initialize urns with standard adjacency matrix
urns = initialize_polya_urns(noisy, adjacency, starting_balls);