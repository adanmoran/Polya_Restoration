%% Reset workspace
clear; close all; clc;

%% Load Binary Image
lena = imread('..\images\lena512.bmp');
lena_bw = imbinarize(lena, 0.52); % binarize image

%% Precomputation before Polya Model
% Add Gaussian noise to binarized image
lena_bw_noise = add_gaussian_noise(lena_bw, 1, 0, 0.8);
figure;
imshowpair(lena_bw, lena_bw_noise, 'montage');

% Find edges in noisy image
edges = edge(lena_bw_noise, 'canny', 0.8, 0.9);
figure;
imshowpair(lena_bw_noise, edges, 'montage');

% Get image initial adjacency matrix with R = 1, Norm = 1
adjacency = get_sparse_adj(size(lena_bw), 1, 1);

% Remove adjacency connections based on edge graph
%adjacency = remove_edge_connections(adjacency, edges);

%% Setup Polya Model
num_iterations = 1000;

% Initialize urns
% urns = initialize_poly_urns(adjacency);
urns = ones(length(lena_bw), 2);
Delta = eye(size(urns,2)) * 2;

for n = 1:num_iterations+1
    urns = polya(urns, adjacency, Delta);
end