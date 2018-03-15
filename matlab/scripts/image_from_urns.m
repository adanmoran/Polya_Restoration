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

    % Multiple colours:
    for i = 1:image_size(1)
       for j = 1:image_size(2)
          row = image_size(2)*(i-1) + j;
          % Find the colours that have the most balls in the urn. There may
          % be more than one colour that tie for first place.
          thisRow = urns(row,:);
          mostBallLocations = find(thisRow == max(thisRow));
          % Note that the range is from 1 - 256

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