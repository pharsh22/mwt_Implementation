clc;
clear all;
close all;

fileID = fopen('input.txt', 'r');
formatSpec = '%f %f';
sizeA = [2 inf];

A = fscanf(fileID,formatSpec,sizeA);
A = A';
[m,n] = size(A);
A = A-0.5;

figure;
hold on;
scatter(A(:,1),A(:,2), 'filled');
xlabel('x');
ylabel('y');
tickValues = 0:1:12;
set(gca, 'XTick', tickValues);
set(gca, 'YTick', tickValues);
grid on;
axis([0 12 0 10]);




fileID = fopen('mwtOutputSorted.txt', 'r');
formatSpec = '%f %f %f %f';
sizeA = [4 inf];

A = fscanf(fileID,formatSpec,sizeA);
A = A';
[m,n] = size(A);

for i=1:m
    x = [A(i,1) A(i,3)];
    y = [A(i,2) A(i,4)];

    hold on;
    plot(x,y);
end
hold off;