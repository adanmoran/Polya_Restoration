function seg_I = quantize_image(image, numBallTypes)
    % Get the numerical type of the image
    classname = class(image); 
    % Get the max size of that type, plus one for zero
    numberOfColours = double(intmax(classname)) + 1;
    
    % Uniform Quantization
    seg_I = floor(image ./ (numberOfColours/numBallTypes));
end