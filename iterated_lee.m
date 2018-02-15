%Borrowed and adapted from Aaron Johnsy, February 2017
%Source: https://angeljohnsy.blogspot.com/2014/08/lee-filter.html

I = imread('lena512.bmp');
J = imnoise(I,'speckle',0.2);
tic;
K = Lee_filter_C(I,J,[2 2]);
c = 15;
for d = 1:c
    K = Lee_filter_C(I,K,[2 2]);
end
toc;
imshowpair(J, K, 'montage')