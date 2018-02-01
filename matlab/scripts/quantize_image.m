%% Quantize Image
% Quantizes input image from original pixel space to labels from 0 to
% number of ball types.
%
% Inputs:
% image = image matrix to be quantized
% num_ball_types = the number of levels to quantize the image into
% q_type = the type of quantization to perform, can be uniform, or
%   non-uniform using the Lloyd-Max quantization
%   inputs can be 'unif' or 'uniform', 'norm' or 'normal, 'exp' or
%   'exponential'
%
% Outputs:
% q_image = quantized image matrix
% partition = the bands where values are partitioned into
% codebook = the "average" values within the bands based on the
%   quantization type/distribution

function [q_image, partition, codebook] = quantize_image(image, num_ball_types, q_type)    
    % Get the numerical type of the image
    classname = class(image); 
    % Get the max size of that type
    num_colours = double(intmax(classname));
    
    % If the number of balls corresponds to no quantization, exit
    if (num_ball_types == num_colours + 1)
        q_image = image;
        partition = 0:num_colours;
        codebook = partition;
        return
    end
    
    if strcmp(q_type, 'unif') || strcmp(q_type, 'uniform')
        % Uniform Quantization
        % Divide the space into equal partitions
        p = num_colours / num_ball_types;
        partition = p:p:num_colours - p;
        
        domain = 0:p:num_colours;
        % Get average between two points, used for 'mid' inverse
        % quantization
        codebook = mean([domain(1:end-1); domain(2:end)]);
        
    else 
        % Lloyd-Max Quantization
        Y = distribution_sampling(q_type) * num_colours; % scale to number of colours
        [partition, codebook] = lloyds(Y, num_ball_types);
        
    end
    
    partition = round(partition);
    codebook = round(codebook);
    q_image = imquantize(image, partition, 0:num_ball_types - 1);
end

%% Distribution Sampling
% Samples the pdf of a distribution for the Lloyd-Max quantization method
%
% Inputs:
% type = distribution type to be sampled
%   can be 'norm' or 'normal', or 'exp' or 'exponential'
%
% Outputs:
% sample = sampled Y values from the input distribution

function sample = distribution_sampling(type)
    % Sampling a Normal distribution with mean 0, variance 1/sqrt(2pi)
    if strcmp(type, 'norm') || strcmp (type, 'normal')
        sample = normpdf(-3:.1:3, 0, 1 / sqrt(2 * pi));
    
    % Sampling an Exponential distribution
    elseif strcmp(type, 'exp') || strcmp (type, 'exponential')
        sample = exppdf(0:.1:6, 1);
        
    end
end