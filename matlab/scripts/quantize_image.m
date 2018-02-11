%% Quantize Image
% Quantizes input image from original pixel space to labels from 0 to
% number of ball types.
%
% Inputs:
% image = image matrix to be quantized
% num_ball_types = the number of levels to quantize the image into
% q_type = the type of quantization to perform, can be uniform, or
%   non-uniform using the Lloyd-Max quantization
%   inputs can be 'unif', 'lloyd'
%
% Outputs:
% q_image = quantized image matrix
% partition = the bands where values are partitioned into
% codebook = the "average" values within the bands based on the
%   quantization type/distribution

function [q_image, partition, codebook] = quantize_image(...
                                                        image, ...
                                                        num_ball_types, ...
                                                        q_type)    
    if islogical(image)
        num_colours = 2;
    else
        % Get the numerical type of the image
        classname = class(image); 
        % Get the max size of that type
        num_colours = double(intmax(classname));
    end
    
    % If the number of balls corresponds to no quantization, exit
    if (num_ball_types == num_colours + 1)
        q_image = image;
        partition = 0:num_colours;
        codebook = partition;
        return
    end
    
    % convert image to vector for codebook calculation
    Y = double(reshape(image, numel(image), 1));
    
    if strcmp(q_type, 'unif')
        % Uniform Quantization
        % Divide the space into equal partitions
        p = num_colours / num_ball_types;
        partition = p:p:num_colours - p;
        
    else 
        % Lloyd-Max Quantization
        [partition, codebook] = lloyds(Y, num_ball_types);
    end
    
    q_image = imquantize(image, partition, 0:(num_ball_types - 1));
    
    if strcmp(q_type, 'unif')
        Y_q = double(reshape(q_image, numel(q_image), 1));
        % For 'mid' inverse quant, use the mean of each bin
        codebook = accumarray(Y_q + 1, Y, [], @mean)';
    end
    % Floor the partition because rounding gives the wrong bounds
    partition = floor(partition);
    % Round the codebook - a ball type of "12.9" should be colour 13
    codebook = round(codebook);
end