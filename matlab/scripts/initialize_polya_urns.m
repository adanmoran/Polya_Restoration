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

    % Initially create the urns
    urns = zeros(length(adjacency), 2);
    
    for i = 1:size(image, 1)
       for j = 1:size(image, 2)
          row = size(image,1)*(i-1) + j;
          % if it's black, add initial_balls to the black column
          if image(i,j) == 0
              urns(row, 1) = initial_balls;
          else % If it's white, add initial_balls to the white column
             urns(row, 2) = initial_balls; 
          end
       end
    end
    
    % The state at time 0 is the initial superurn
    urns = adjacency * urns;
end