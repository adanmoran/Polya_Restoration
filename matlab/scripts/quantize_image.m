function [q_image, partition, codebook] = quantize_image(image, num_ball_types, q_type)    
    % Get the numerical type of the image
    classname = class(image); 
    % Get the max size of that type
    num_colours = double(intmax(classname));
    
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
        % Get average between two points
        codebook = mean([domain(1:end-1); domain(2:end)]);
        
    else 
        % Lloyd-Max Quantization
        % Sampling a Normal distribution with mean 0, variance 1/sqrt(2pi)
        % and scaled to the number of colours
        Y = distribution_sampling(q_type) * num_colours;
        [partition, codebook] = lloyds(Y, num_ball_types);
        
    end
    
    partition = round(partition);
    codebook = round(codebook);
    q_image = imquantize(image, partition, 0:num_ball_types - 1);
end

function sample = distribution_sampling(type)
    if strcmp(type, 'norm') || strcmp (type, 'normal')
        sample = normpdf(-3:.1:3, 0, 1 / sqrt(2 * pi));
        
    elseif strcmp(type, 'exp') || strcmp (type, 'exponential')
        sample = exppdf(0:.1:6, 1);
        
    end
end