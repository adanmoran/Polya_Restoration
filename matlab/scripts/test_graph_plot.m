clear all;
close all;
clc;

tic;
% Create a Polya Model graph
V = [5 5; 
     6 4; 
     7 3; 
     4 6; 
     3 7];
E = [ 0 1 0 0 0;
      0 0 1 0 0;
      0 0 0 1 1;
      0 0 0 0 1;
      0 0 0 0 0];
E = E + E' + eye(5);
Delta = eye(size(V,2))*2;

% Number of Polya iterations
N = 50;

% Create a graph which we can see with plotting
G = graph(E, 'OmitSelfLoops');

% Plot the graph and make the nodes more visible
f1 = figure;
p = plot(G, 'MarkerSize', 10);

% Create a video to watch it later
videoLength = 20; % seconds
videoName = 'polya.avi';
outputVideo = VideoWriter(fullfile('frames', videoName));
outputVideo.FrameRate = N/videoLength;

open(outputVideo)

% Test the Polya Scheme on the graph
for n = 1:N+1
    % Colour each node and label it with its urn contents
    for i = 1:length(V)
        s = sum(V(i,:));
        highlight(p, i, 'NodeColor', [ V(i,1)/s 0 V(i,2)/s]);
        p.NodeLabel{i} = strcat('(',num2str(V(i,1)),'r ,', num2str(V(i,2)), 'b)');
    end

    % Create a video frame
    writeVideo(outputVideo, frame2im(getframe(f1)));
    
    % Make Polya run, but skip the last time since after the N'th frame
    % we aren't saving frames
    if (n ~= N+1)
        V = polya(V, E, Delta);
    end
end

%Ensure the video is closed properly - DO NOT REMOVE
close(outputVideo)

toc