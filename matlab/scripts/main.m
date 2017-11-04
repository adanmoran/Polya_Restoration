%% Reset workspace
clear; close all; clc;

%% Declare Variables/Parameters
% Image Things
image_location = '..\images\lena512.bmp';
image_binarize_thresh = 0.52;

% Gaussian Noise Parameters
gaussian_std_dev = 1;
gaussian_mean = 0;
gaussian_confidence_interval = 0.8;

% Edge Detection Parameters
edge_type = 'canny';
edge_thresh = 0.8;
edge_sigma = 0.9;

% Adjacency Matrix Parameters
adj_radius = 1;
adj_norm = 1;                   % can be 1, 2, or 'inf'

% Polya Parameters
polya_iterations = 2;

%% Load Binary Image
image = imread(image_location);
image_bw = imbinarize(image, image_binarize_thresh); % binarize image

%% Precomputation before Polya Model
% Add Gaussian noise to binarized image
lena_bw_noise = add_gaussian_noise(image_bw,            ... 
                                   gaussian_std_dev,    ... 
                                   gaussian_mean,       ...
                                   gaussian_confidence_interval);
figure;
imshowpair(image_bw, lena_bw_noise, 'montage');

% Find edges in noisy image
edges = edge(lena_bw_noise, edge_type, edge_thresh, edge_sigma);
figure;
imshowpair(lena_bw_noise, edges, 'montage');

% Get image initial adjacency matrix with R = 1, Norm = 1
adjacency = get_sparse_adj(size(image_bw), adj_radius, adj_norm);

% Remove adjacency connections based on edge graph
%adjacency = remove_edge_connections(adjacency, edges);

%% Setup Polya Model
% Initialize urns
% urns = initialize_poly_urns(adjacency);
urns = ones(length(image_bw) ^ 2, 2);
Delta = eye(size(urns,2)) * 2;

for n = 1:polya_iterations+1
    urns = polya(urns, adjacency, Delta);
end