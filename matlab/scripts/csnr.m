%% Channel Signal to Noise Ratio (CSNR) - csnr(ref, channel_noise)
% Calculate the channel signal to noise ratio in dB.
% csnr = 10log_10(sigma_ref / sigma_noise)
% 
% Inputs:
%   - ref: the reference image to compare against
%   - channel_noise: the variance of the channel noise
% 
% Outputs:
%   - csnr: the calculated CSNR of the reference image to the channel noise
function csnr = csnr(ref, channel_noise)
    sigma_I = var(double(ref(:)));
    
    csnr = 10 * log10(sigma_I / channel_noise);
end