points = [2.5, 1.0;
4.0, 2.1;
3.4, 4.2;
1.6, 4.2;
1.0, 2.1];
polygon = polyshape(points(:, 1),points(:, 2));
segment = [1.4, 2.75;
3.6, 2.2];

figure;
plot(polygon);
hold on;
plot(segment(:, 1), segment(:, 2), 'r--');
hold on;
plot(points(:, 1),points(:, 2), 'ko');
hold on;
plot(segment(:, 1), segment(:, 2), 'r*');
