points = [0.0, 0.0;
4.0, 0.0;
2.0, 2.0];
polygon = polyshape(points(:, 1),points(:, 2));
segment = [-1.0, -1.0;
1.0, 1.0];

figure;
plot(polygon);
hold on;
plot(segment(:, 1), segment(:, 2), 'r--');
hold on;
plot(points(:, 1),points(:, 2), 'ko');
hold on;
plot(segment(:, 1), segment(:, 2), 'r*');
