function image = inverse_quantize_image(q_image, inverse_quantization, partition)
    % Get the numerical type of the image
    classname = class(q_image); 
    % Get the max size of that type
    num_colours = double(intmax(classname));
    
    if (length(partition) == num_colours + 1)
        image = uint8(partition(q_image));
        return
    end

    if strcmp(inverse_quantization, 'low') || strcmp(inverse_quantization, 'lower')
        partition = [0 partition];
    
    elseif strcmp(inverse_quantization, 'high') || strcmp(inverse_quantization, 'upper')
        partition = [partition num_colours];
    end
    
    % B(A) It treats the elements of A as indices into B and returns 
    % an array with the same size as A.
    image = uint8(partition(q_image));
end