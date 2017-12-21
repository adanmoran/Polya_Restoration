%% Add Gaussian Black and White Noise to an Image
% This function will add Gaussian noise to a binarized image.
%
% Inputs:
%   - image: The image to be noisified.
%   - sigma: The standard deviation of the Gaussian noise to be added
%   - mean: The mean of the Gaussian noise to be added
%   - confidence interval: The threshold for a pixel to be changed due to
%   noise, i.e., how likely a pixel is to stay the same (probability)
% 
% Outputs:
%     - image: The noisified image.

% ex. image = gaussian_noise(image, 1, 0, 0.8);
function image = add_gaussian_noise(image, sigma, mean, confidence_interval)
    rng(0, 'twister'); % Choose your random generator to use randn()

    for i = 1:size(image,1)
        for j = 1:size(image,2)
            % If you are outside the confidence interval, flip the pixel.
            if (sigma*randn(1,1) + mean) > confidence_interval
                if image(i,j) > 0
                    image(i,j) = 0;
                else
                    image(i,j) = 1;
                end
            end
        end
    end
end