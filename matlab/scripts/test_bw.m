%% Load Lena
lena = imread('..\images\lena512.bmp');
% Convert to black and white
%BW = im2bw(lena, 0.52);

% im2bw is not recommended by MATLAB - using imbinarize instead
BW = imbinarize(lena, 0.52);

% % Change logical to 255-range to allow for noise
% B = uint8(BW);
% for i = 1:size(B,1)
%     for j = 1:size(B,2)
%         if B(i,j) == 1
%             B(i,j) = 255;
%         end
%     end
% end

%% Add Gaussian noise to the image
rng(0, 'twister'); % Choose your random generator to use randn()
C = BW;
% Gaussian noise values
sigma = 1;
mean = 0;
% how likely a pixel is to stay the same (probability)
confidence_interval = 0.8;

for i = 1:size(C,1)
    for j = 1:size(C,2)
        % If you are outside the 95% confidence interval, flip the pixel.
        if (sigma*randn(1,1) + mean) > confidence_interval
            if C(i,j) > 0
                C(i,j) = 0;
            else
                C(i,j) = 1;
            end
        end
    end
end

figure;
imshowpair(BW, C, 'montage');
%% Take the Edge Map of Lena

% The variance of Canny's gaussian filter. Default is sqrt(2)
sigma = 2.7;
% Threshold for the Canny method
thresh = 0.4;
edges = edge(C, 'canny', thresh, sigma);
figure;
imshowpair(C, edges, 'montage');
