%% Calculate the Equivalent Number of Looks (ENL)
% Inputs:
% original - the original image before noise was added
% polya - output of the polya filter, on an image that had Rayleigh speckle
% noise
% medianed - output of medfilt2, on an image that had Rayleigh speckle
% noise
%
% Outputs:
% polya_enl - the ENL for the polya filter
% median_enl - the ENL for the median filter
%
% The speckle noise for a Rayleigh Distribution has variance 0.275/N, where
% N is the number of SAR looks. The resulting noisy image is
% -- noisy(i,j) = original(i,j) .* noise(i,j)
% 
% As such, if the original is known, one can compute the equivalent number
% of looks by getting the noise and computing the variance:
% -- noise(i,j) = filtered(i,j) ./ original(i,j)
% -- ENL = 0.275 / var(noise)
%
% Example usage:
% original = imread('../images/aerial1.tiff');
% polya =
% imread('./frames/aerial1_2norm_edge/aerial1_speckle_low_optimal.tiff');
% medianed =
% imread('./frames/aerial1_2norm_edge/aerial1_speckle_low_optimal_median_2N.tiff');
% [polya_enl, median_enl] = enl(image, polya, medianed)

function [polya_enl, median_enl] = enl(original, polya, medianed)
    % Get non-zero locations of the original image, as any locations where
    % the image was zero have noise value of zero (and thus can be ignored
    % when computing the noise values)
    image_nonzero = original ~= 0;

    % Find the noise on the polya image
    polya_diff = double(polya(image_nonzero)) ./ ...
        double(original(image_nonzero));
    % Compute the ENL of the polya image
    polya_sigma = var(polya_diff(:));
    polya_enl = 0.275/polya_sigma;
    
    % Find the noise on the polya image
    median_diff = double(medianed(image_nonzero)) ./ ...
        double(original(image_nonzero));
    % Compute the ENL of the median image
    median_sigma = var(median_diff(:));
    median_enl = 0.275/median_sigma;
end

