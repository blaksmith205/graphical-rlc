function returnVals = calculate_transient(R, L, C, y_init, dy_init, finalVal, isSeries, isStep)
syms C1 C2 sourceVal t s1 s2 a wd;
%CALCULATE_TRANSIENT Calculates and displays the transient response

% Obtain the roots
roots = calc_roots(R, L, C, isSeries);

% Determine the response
response = determine_response(roots);

% Obtain the rhs
rhs = equation_of(response);

% Replace final value
rhs = subs(rhs, sourceVal, finalVal);
% Replace s1 and s2 if in equation
rhs = subs(rhs, s1, roots(1));
rhs = subs(rhs, s2, roots(2));
% Replace wd if in equation
rhs = subs(rhs, wd, roots(3));
% Replace alpha if in equation
rhs = subs(rhs, a, roots(4));
% Obtain derivative
diff_rhs = diff(rhs, t);
% Obtain equations based on initial conditions
rhs0 = subs(rhs, t, 0);
diff_rhs0 = subs(diff_rhs, t, 0);
equation = y_init == rhs0;
diff_equation = dy_init == diff_rhs0;

% solve the equations
solutions = solve([equation diff_equation], [C1 C2]);

% Replace coefficants
rhs = subs(rhs, C1, solutions.C1);
rhs = subs(rhs, C2, solutions.C2);

% Convert symbolic function to normal function
real_eq = matlabFunction(rhs);
% Show the response over 5 seconds
timeVals = linspace(0, 2e-3, 2000);
yVals = arrayfun(real_eq, timeVals);
plot(timeVals, yVals);

% Setup the return values
fields = ["s1" "s2" "wd" "response" "rhs"];
returnVals = struct(fields(1), roots(1), fields(2), roots(2), fields(3), roots(3), fields(4), response, fields(5), rhs);
end

