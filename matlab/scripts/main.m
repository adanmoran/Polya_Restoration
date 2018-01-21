%% Reset workspace
%clear; 
close all; 
clc;

%% Declare Variables/Parameters
% Verbosity flags
verbose = true;

% Image Things
%image_location = '..\images\resolution_chart.tiff';
% image_location = '..\images\lena512.bmp';
image_location = '..\images\oil_spill.jpg';

% Lena threshold: 0.52
% oil_spill threshold: 0.18
image_binarize_thresh = 0.18;

% Choose the noise type:
% Options are 'gaussian' or 'none' or 'burst'.
noise_type = 'gaussian';

% Gaussian Noise Parameters
gaussian_std_dev = 1;
gaussian_mean = 0;
gaussian_confidence_interval = 0.8; % Error rate is 1 - this

% Burst Noise parameters
burst_correlation = 0.98;
burst_error = 0.1;

% Edge Detection Parameters
edge_type = 'canny';
edge_thresh = 0.4;
edge_sigma = 2.7;
use_edges = false;

% Adjacency Matrix Parameters
adj_radius = 2;
adj_norm = 1;                   % can be 1, 2, or 'inf'

% Urn parameters
starting_balls = 10; % the initial number of balls at time -1 of each urn

% Polya Parameters
polya_iterations = 20;
delta_black = 5;
delta_white = delta_black;

% Choose how to sample
% Options are: 'median' or 'random'
sample_type = 'median';

% Median Filter parameters
median_iterations = 3; % Don't need much, generally after 4 it converges

%% Load Binary Image
image = imread(image_location);

% If it's an rgb image, convert it to grayscale
% i.e. if the image is "3D" we convert it to grayscale
if size(size(image),2) > 2
    image = rgb2gray(image);
end
image_bw = imbinarize(image, image_binarize_thresh); % binarize image

%% Precomputation before Polya Model
% Add Gaussian noise to binarized image
image_bw_noise = add_gaussian_noise(image_bw,            ... 
                                   gaussian_std_dev,    ... 
                                   gaussian_mean,       ...
                                   gaussian_confidence_interval);
if strcmp(noise_type, 'none') || strcmp(noise_type, '')
   image_bw_noise = image_bw;
elseif strcmp(noise_type, 'burst')
    image_bw_noise = add_bursty_noise(image_bw,          ...
                                      burst_correlation, ...
                                      burst_error);
end

figure;
imshowpair(image_bw, image_bw_noise, 'montage');
title('Noise added to Original');

% Find edges in noisy image
edges = edge(image_bw_noise, edge_type, edge_thresh, edge_sigma);
figure;
imshowpair(image_bw_noise, edges, 'montage');
title('Edge Map');

% Get image initial adjacency matrix with R = 1, Norm = 1
adjacency = get_sparse_adj(size(image_bw), adj_radius, adj_norm);

%% Setup Polya Model
% Initialize urns with standard adjacency matrix
urns = initialize_polya_urns(image_bw_noise, adjacency, starting_balls);

% Remove adjacency connections based on edge graph
%adjacency = remove_edge_connections(adjacency, edges);
if use_edges
    adjacency = adjacency_minus_edge_d(adjacency, edges, adj_radius);
end

% Initialize Delta for the polya urns
Delta = [delta_black      0      ;
              0      delta_white ];

% Vector of errors
Pe = zeros(polya_iterations + 1, 1);
Pe(1) = compute_error(image_bw, image_bw_noise);


% Iterate the polya contagion over the urns
for n = 1:polya_iterations
    tic
    urns = polya(urns, adjacency, Delta, sample_type);
    duration = toc;
    % Compute the image from the resulting urns
    if verbose
        output = image_from_urns(size(image_bw_noise), urns);
        % Show the probability of error
        Pe(n+1) = compute_error(image_bw, output);
        fprintf('n = %d | Pe = %.6f | Duration: %.2f\n', n, Pe(n+1), duration);
    else % Display the runtime so we know it's working
        toc
    end
end

if verbose
    % Plot of errors
    figure;
    plot(0:length(Pe)-1, Pe);
    title('Pe vs N');
    xlabel('N');
    ylabel('Pe (%)');
end

% Display the images
output = image_from_urns(size(image_bw_noise), urns);

figure;
imshowpair(image_bw_noise, output, 'montage');
title('Noise vs NPCM');

figure;
imshowpair(image_bw, output, 'montage');
title('Original vs NPCM');

%% Median Filter Comparison

medianed = image_bw_noise;
for i = 1:median_iterations
   medianed = medfilt2(medianed); 
end

% Show the pictures
figure;
imshowpair(image_bw, medianed, 'montage');
title('Original vs Median');

% Error compared to image
fprintf('Median Error for %d iterations: %f\n', median_iterations, ...
    compute_error(image_bw, output));

fprintf('Difference between Polya and Median: %f\n', ...
    compute_error(output, medianed));