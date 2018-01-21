%% Image From Urns
% Convert an urn array into an image
%
% Inputs:
% image_size - the size of the black and white image.
% urns - an urn array that is length(image)*width(image)x2, where (i,1) is
% the number of black bals and (i,2) is the number of white balls in pixel
% i, where the pixels are counted along each row.
%
% Returns:
% image - an image representing the urn state
function image = image_from_urns(image_size, urns)
    image = zeros(image_size);

%     % Find the first maximum value along each row and get its index
%     [~,indicesWithMostBalls] = max(urns, [], 2);
%     % Convert the index locations to colour values by subtracting one
%     mostBalls = indicesWithMostBalls - 1;
%     % Convert to an image by reshaping this vector
%     image = reshape(mostBalls, image_size)';
    
    % Multiple colours:
%         
        for i = 1:image_size(1)
           for j = 1:image_size(2)
              row = image_size(2)*(i-1) + j;
              % Find the colours that have the most balls in the urn. There may
              % be more than one colour that tie for first place.
              thisRow = urns(row,:);
              mostBallLocations = find(thisRow == max(thisRow));
              % Subtract ones to make it into a 0-255 range (instead of 1-256)
              mostBallLocations = mostBallLocations - 1;

              % If there is only one colour, this pixel becomes that colour
              if length(mostBallLocations) == 1
                  image(i,j) = mostBallLocations;
              % If theres a tie, take the darkest colour as default. This may
              % be changed later
              else
                  image(i,j) = ceil(median(mostBallLocations));
              end
           end
        end
end

%% Old Versions:
% Black and white only
%         for i = 1:image_size(1)
%            for j = 1:image_size(2)
%               row = size(image,1)*(i-1) + j;
%               % If there are more black balls, this is a black pixel
%               if urns(row, 1) >= urns(row, 2)
%                   image(i,j) = 0;
%               % If there are more white balls, this is a white pixel
%               else
%                   image(i,j) = 1;
%               end
%            end
%         end

% Multiple colours:
%         
%         for i = 1:image_size(1)
%            for j = 1:image_size(2)
%               row = size(image,1)*(i-1) + j;
%               % Find the colours that have the most balls in the urn. There may
%               % be more than one colour that tie for first place.
%               thisRow = urns(row,:);
%               mostBallLocations = find(thisRow == max(thisRow));
%               % Subtract ones to make it into a 0-255 range (instead of 1-256)
%               mostBallLocations = mostBallLocations - 1;
% 
%               % If there is only one colour, this pixel becomes that colour
%               if length(mostBallLocations) == 1
%                   image(i,j) = mostBallLocations;
%               % If theres a tie, take the darkest colour as default. This may
%               % be changed later
%               else
%                   image(i,j) = min(mostBallLocations);
%               end
%            end
%         end