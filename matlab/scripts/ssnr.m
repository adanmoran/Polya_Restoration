%% Signal to Speckle Noise Ratio (SSNR) - ssnr(ref, speckle)
% Calculate the SSNR of an image with speckle noise, compared to a
% reference image in dB.
% ssnr = 10log_10(sigma_ref / sigma_speckleError)
% 
% Inputs:
%   - ref: the reference image to compare against
%   - speckle: the image with speckle noise
% 
% Outputs:
%   - ssnr: the calculated SSNR of the reference image to the speckled
%   image
function ssnr = ssnr(ref, speckle)
    ref = double(ref);
    sigma_I = var(ref(:));
    sigma_S = immse(double(speckle), ref);
    
    ssnr = 10 * log10(sigma_I / sigma_S);
end