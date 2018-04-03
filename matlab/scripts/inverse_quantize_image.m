%% Inverse Quantize Image
% Maps the input quantized image based on the partitions of the
% quantization. Can be mapped to the floor, ceiling, or mid point of the
% partition.
%
% Inputs:
% q_image = the quantized image
% inverse_quantization = the type of inverse quantization to be performed
%   can be 'low', 'high', or 'mid'
% partition = the bands where the values are partitioned into
% codebook = the "average" values within the bands based on the
%   quantization type/distribution
%
% Outputs:
% image = the quantized image mapped back to its partitioned space

function image = inverse_quantize_image(q_image, ...
                                        inverse_quantization, ...
                                        partition, ...
                                        codebook)
    if islogical(q_image)
        num_colours = 2;
    else
        % The maximum greyscale value is 255.
        num_colours = 255;
    end
    
    % If the number of balls corresponds to no quantization, exit
    if (length(partition) == num_colours + 1)
        image = uint8(partition(q_image));
        return
    end

    if strcmp(inverse_quantization, 'low')
        assigned_values = [0 partition];
    
    elseif strcmp(inverse_quantization, 'high')
        assigned_values = [partition num_colours];
        
    elseif strcmp(inverse_quantization, 'mid')
        assigned_values = codebook;
        
    end
    
    % B(A) It treats the elements of A as indices into B and returns 
    % an array with the same size as A.
    image = uint8(assigned_values(q_image));
end