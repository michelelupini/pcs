points = [1.0, 1.0;
5.0, 1.0;
5.0, 3.1;
1.0, 3.1];
polygon = polyshape(points(:, 1),points(:, 2));
segment = [2.0, 1.2;
4.0, 3.0];

figure;
plot(polygon);
hold on;
plot(segment(:, 1), segment(:, 2), 'r--');
hold on;
plot(points(:, 1),points(:, 2), 'ko');
hold on;
plot(segment(:, 1), segment(:, 2), 'r*');
