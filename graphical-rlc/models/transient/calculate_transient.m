function [root1, root2, damped_freq, response, rhs] = calculate_transient(R, L, C, y_init, dy_init, finalVal, isSeries, isStep, outputName)
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
figure('visible', 'off');
plot(timeVals, yVals);
xlabel('Time (secs)');
if (isStep == true && isSeries == false) || (isStep == false && isSeries == true)
    ylabel('Amps');
elseif (isStep == true && isSeries == true) || (isStep == false && isSeries == false)
    ylabel('Volts');
end
print(outputName, '-dpng');

% Setup the output
root1 = roots(1);
root2 = roots(2);
damped_freq = roots(3);
rhs = char(rhs);
end

