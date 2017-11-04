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
edge_thresh = 0.4;
edge_sigma = 2.7;

% Adjacency Matrix Parameters
adj_radius = 1;
adj_norm = 1;                   % can be 1, 2, or 'inf'

% Urn parameters
starting_balls = 10; % the initial number of balls at time -1 of each urn

% Polya Parameters
polya_iterations = 25;
delta_black = 5;
delta_white = delta_black;

%% Load Binary Image
image = imread(image_location);
image_bw = imbinarize(image, image_binarize_thresh); % binarize image

%% Precomputation before Polya Model
% Add Gaussian noise to binarized image
image_bw_noise = add_gaussian_noise(image_bw,            ... 
                                   gaussian_std_dev,    ... 
                                   gaussian_mean,       ...
                                   gaussian_confidence_interval);
figure;
imshowpair(image_bw, image_bw_noise, 'montage');

% Find edges in noisy image
edges = edge(image_bw_noise, edge_type, edge_thresh, edge_sigma);
figure;
imshowpair(image_bw_noise, edges, 'montage');

% Get image initial adjacency matrix with R = 1, Norm = 1
adjacency = get_sparse_adj(size(image_bw), adj_radius, adj_norm);

% Remove adjacency connections based on edge graph
%adjacency = remove_edge_connections(adjacency, edges);
tic
adjacency = adjacency_minus_edge_1(adjacency, edges);
toc

%% Setup Polya Model
% Initialize urns
urns = initialize_polya_urns(image_bw_noise, adjacency, starting_balls);
% Initialize Delta for the polya urns
Delta = [delta_black      0      ;
              0      delta_white ];

% Vector of errors
Pe = zeros(polya_iterations + 1, 1);
Pe(1) = compute_error(image_bw, image_bw_noise);

% Iterate the polya contagion over the urns
for n = 1:polya_iterations
    urns = polya(urns, adjacency, Delta);
    % Compute the image from the resulting urns
    output = image_from_urns(size(image_bw_noise), urns);
    % Show the probability of error
    Pe(n+1) = compute_error(image_bw, output);
	fprintf('n = %d | Pe = %.6f\n', n, Pe(n+1));
end

% Plot of errors
figure;
plot(0:length(Pe)-1, Pe);
title('Pe vs N');
xlabel('N');
ylabel('Pe (%)');

% Display the images
figure;
imshowpair(image_bw_noise, output, 'montage');
figure;
imshowpair(image_bw, output, 'montage');