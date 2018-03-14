%% polyafilt
% Greyscale image restoration using a polya process as a filter.
% # Find the Edge Map
% # Quantize Image
% # Build Adjacency Matrix
% # Setup Polya Model
% # Iterate the Polya Model
% # Build the Final Image
%
% Inputs:
% noisy_image - the noisy image to be restored (as a matrix)
% prefs - preferences structure
%   e.g.
%       prefs.edges.use_edges = false;
%       prefs.edges.filter = 'canny';
%       % The variance of Canny's gaussian filter. Default is sqrt(2)
%       prefs.edges.sigma = 3; 
%       % Threshold for the Canny method
%       % Oil Spill: 0.4
%       % Lena: with gauss = 0.2, with burst = 0.38
%       % Two ships: 0.5
%       prefs.edges.thresh = 0.2;
% 
%       prefs.quant.num_ball_types = 30; % [2 - 256]
%       prefs.quant.type = 'norm'; % unif, norm, exp
%       prefs.quant.inverse = 'high'; % low, high, mid
% 
%       prefs.adj.radius = 3;
%       prefs.adj.norm = 2;
% 
%       prefs.polya.sample_type = 'median'; % 'median', 'random'
%       prefs.polya.starting_balls = 100; % Starting balls in each urn
%       prefs.polya.balls_to_add = 60; % Number of balls to add to the urn 
%                                      % after each polya step
%       prefs.polya.iterations = 8;
%
% Outputs:
% output_image - the restored greyscale image (as a matrix)

function output_image = polyafilt(noisy_image, prefs)
    %% Find the Edge Map
    if prefs.edges.use_edges
        edges = edge(noisy_image, ...
                     prefs.edges.filter, ...
                     prefs.edges.thresh, ...
                     prefs.edges.sigma);
        figure;
        imshowpair(noisy_image, edges, 'montage');
    end

    %% Quantize Image
    [noisy_image, partition, codebook] = quantize_image(...
                                            noisy_image, ... 
                                            prefs.quant.num_ball_types, ... 
                                            prefs.quant.type);

    %% Build Adjacency Matrix
    % Get image adjacency matrix
    adjacency = get_sparse_adj(size(noisy_image), ...
                               prefs.adj.radius, ...
                               prefs.adj.norm);

    %% Setup Polya Model
    % Matrix for ball addition
    Delta = prefs.polya.balls_to_add * speye(prefs.quant.num_ball_types);
    % Initialize urns
    urns = initialize_polya_urns(noisy_image, ...
                                 prefs.polya.starting_balls, ...
                                 prefs.quant.num_ball_types);

    % Remove adjacency connections based on edge graph
    if prefs.edges.use_edges
        adjacency = adjacency_minus_edge_d(adjacency, ...
                                           edges, ...
                                           prefs.adj.radius);
    end
    %% Iterate the Polya Model
    if prefs.video.save_video
        % Open a video maker
        location = fullfile( prefs.video.folder, ...
                             prefs.video.name);
        fprintf('Saving results to video at %s\n', location);
        outputVideo = VideoWriter();
        outputVideo.FrameRate = prefs.video.frame_rate;
        open(outputVideo);
        % Loop through the polya process and save each frame
        for i = 1:prefs.polya.iterations
            tic
            fprintf('Iteration %d of %d | Duration: ', ...
                    i, ...
                    prefs.polya.iterations);
            urns = polya(urns, adjacency, Delta, prefs.polya.sample_type);
            fprintf('%.3f\n', toc);
            % Create a frame for the video
            fprintf('------ Generating video frame %d', N);
            writeVideo(outputVideo, ...
                       image_from_urns(size(noisy_image), urns));
        end
        % Clean up the video buffer and unlock it from memory
        close(outputVideo);
    else
        urns = polya_eigen(urns, adjacency, Delta, 'median', prefs.polya.iterations);
    end

    %% Build the final image
    output_image = image_from_urns(size(noisy_image), urns);
    output_image = inverse_quantize_image(output_image, ...
                                    prefs.quant.inverse, ...
                                    partition, ...
                                    codebook);
end