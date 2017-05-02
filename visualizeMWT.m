clc;
clear all;
close all;

fileID = fopen('mwtOutput2.txt', 'r');
formatSpec = '%f %f %f %f';
sizeA = [4 inf];

A = fscanf(fileID,formatSpec,sizeA);
A = A';
[m,n] = size(A);


figure;
for i=1:m
    x = [A(i,1) A(i,3)];
    y = [A(i,2) A(i,4)];

    hold on;
    plot(x,y);
end
hold off;