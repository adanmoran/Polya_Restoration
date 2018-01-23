function image = inverse_quantize_image(q_image, num_ball_types, inverse_quantization)
    % Get the numerical type of the image
    classname = class(q_image); 
    % Get the max size of that type, plus one for zero
    numberOfColours = double(intmax(classname)) + 1;

    if strcmp(inverse_quantization, 'mid') || strcmp(inverse_quantization, 'middle')
        image = floor(q_image .* (numberOfColours/(num_ball_types-1)));
    end
end