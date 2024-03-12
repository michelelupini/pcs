points = [1.5, 1.0;
5.6, 1.5;
5.5, 4.8;
4.0, 6.2;
3.2, 4.2;
1.0, 4.0];
polygon = polyshape(points(:, 1),points(:, 2));
segment = [2.0, 3.7;
4.1, 5.9];

figure;
plot(polygon);
hold on;
plot(segment(:, 1), segment(:, 2), 'r--');
hold on;
plot(points(:, 1),points(:, 2), 'ko');
hold on;
plot(segment(:, 1), segment(:, 2), 'r*');
