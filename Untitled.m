filename = 'Data2.csv';
figure(1)
M = csvread(filename)
hold on;grid on;

plot(M(:,1),M(:,4),'*')
plot(M(:,2),M(:,5),'*')
plot(M(:,3),M(:,6),'*')

%%

%%

filename = 'Data2.csv';
figure(3)
M2 = csvread(filename)
hold on;grid on;
xticks(-2:0.01:2)
yticks(-2:0.01:2)
zticks(-2:0.01:2)
plot3(M2(:,1),M2(:,2),M2(:,3))


%%
chord = 0:0.01:1;
tmax=0.09;
tmax./chord