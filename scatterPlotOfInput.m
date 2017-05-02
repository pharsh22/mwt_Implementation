clc;
clear all;
close all;

% x = [1, 2 ,2 ,2 ,4 ,4 ,4 ,5 ,5 ,5 ,6 ,6 ,7 ,8 ,9 ,10 ,11 ,12]-0.5;
% y = [1 1 2 3 1 2 3 2 3 4 2 3 2 5 6 4 7 7]-0.5;

fileID = fopen('input.txt', 'r');
formatSpec = '%f %f';
sizeA = [2 inf];

A = fscanf(fileID,formatSpec,sizeA);
A = A';
[m,n] = size(A);
A = A-0.5;

figure;
scatter(A(:,1),A(:,2), 'filled');
xlabel('x');
ylabel('y');
tickValues = 0:1:12;
set(gca, 'XTick', tickValues);
set(gca, 'YTick', tickValues);
grid on;
axis([0 12 0 10]);