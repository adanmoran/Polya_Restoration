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
    
    if strcmp(q_type, 'unif')
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
        % convert image to vector for lloyd-max algorithm
        Y = double(reshape(image, numel(image), 1));
        [partition, codebook] = lloyds(Y, num_ball_types);
        
    end
    
    partition = round(partition);
    codebook = round(codebook);
    q_image = imquantize(image, partition, 0:num_ball_types - 1);
end