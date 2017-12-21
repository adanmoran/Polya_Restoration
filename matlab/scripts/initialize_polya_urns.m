%% Initialize Polya Urns
% Take the superurn of each pixel as its initial state.
% Each pixel will have initial_balls black balls if it is black,
% or initial_balls white balls if it is white.
%
% Inputs:
% image - the black-and-white image for which you're determining the vector
% of vertices.
% adjacency - the adjacency matrix for the image
% initial_balls - the number of balls to start with at time -1 (before the
% superurn is calculated)
%
% Returns:
% urns - a length(adjacency)x2 vector where (i,1) is the number of
% black balls and (i,2) is the number of white balls in the urn
% related to pixel i.
function urns = initialize_polya_urns(image, adjacency, initial_balls)

    % Default number of balls for a polya process is 2
    numBallTypes = 2;
    
    if ~islogical(image)
        % Create as many ball types as can fit in the numerical type of
        % this image (e.g. 255 for 'uint8').
        
        % Get the numerical type of the image
        classname = class(image); 
        % Get the max size of that type, plus one for zero
        numBallTypes = uint64(intmax(classname)) + 1; 
    end
    % Create the urns
    urns = zeros(length(adjacency), numBallTypes);
    
    for i = 1:size(image, 1)
       for j = 1:size(image, 2)
          row = size(image,1)*(i-1) + j;
          % Add initial_balls to the correct column (offset by one because
          % of MATLAB's indexing system)
          urns(row, image(i,j) + 1) = initial_balls;
       end
    end
    
    % The state at time 0 is the initial superurn
    urns = adjacency * urns;
end