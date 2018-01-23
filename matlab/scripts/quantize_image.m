function q_image = quantize_image(image, num_ball_types, q_type)
    % Get the numerical type of the image
    classname = class(image); 
    % Get the max size of that type, plus one for zero
    numberOfColours = double(intmax(classname)) + 1;
    
    if strcmp(q_type, 'unif') || strcmp(q_type, 'uniform')
        % Uniform Quantization
        q_image = floor(image ./ (numberOfColours/(num_ball_types-1)));
    end
end