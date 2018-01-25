function [q_image, partition] = quantize_image(image, num_ball_types, q_type)
    % Get the numerical type of the image
    classname = class(image); 
    % Get the max size of that type
    num_colours = double(intmax(classname));
    
    if strcmp(q_type, 'unif') || strcmp(q_type, 'uniform')
        % Uniform Quantization
        % Divide the space into equal partitions
        p = num_colours / num_ball_types;
        partition = p:p:num_colours - p;
        
    elseif strcmp(q_type, 'lloyd') || strcmp(q_type, 'non-unif')
        % Lloyd-Max Quantization
        % Sampling a Normal distribution with mean 0, variance 1/sqrt(2pi)
        % and scaled to the number of colours
        Y = normpdf(-3:.1:3, 0, 1 / sqrt(2 * pi)) * num_colours;
        [partition, ~] = lloyds(Y, num_ball_types);
    end
    
    q_image = imquantize(image, partition, 0:num_ball_types - 1);
end