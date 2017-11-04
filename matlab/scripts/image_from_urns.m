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
    
    for i = 1:image_size(1)
       for j = 1:image_size(2)
          row = size(image,1)*(i-1) + j;
          % If there are more black balls, this is a black pixel
          if urns(row, 1) >= urns(row, 2)
              image(i,j) = 0;
          % If there are more white balls, this is a white pixel
          else
              image(i,j) = 1;
          end
       end
    end
end