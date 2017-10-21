lena = imread('../images/lena512.bmp');

%This is to get blurry images. Do stuff. On internet
intImage = integralImage(lena);
avgH = integralKernel([1 1 7 7], 1/49);
J = integralFilter(intImage, avgH);
J = uint8(J);

%This Uses the best edge detection function 'canny'
BW2 = edge(lena,'canny');
BW1 = edge(J,'canny');

%History of Adan's "Code" lol.
figure;imshow(imbinarize(J, 0.52))
figure;imshowpair(imbinarize(J, 0.52), imbinarize(lena, 0.52))
figure;imshowpair(imbinarize(J, 0.52), imbinarize(lena, 0.52), 'montage')
imshowpair(imbinarize(J, 0.5), imbinarize(lena, 0.52), 'montage')
imshowpair(imbinarize(J, 0.4), imbinarize(lena, 0.52), 'montage')
imshowpair(imbinarize(J, 0.58), imbinarize(lena, 0.52), 'montage')
imshowpair(imbinarize(J), imbinarize(lena, 0.52), 'montage')
imshowpair(imbinarize(J), imbinarize(lena), 'montage')
imshowpair(imbinarize(lena, 0.52), edge(imbinarize(lena,0.52), 'canny'), 'montage')
