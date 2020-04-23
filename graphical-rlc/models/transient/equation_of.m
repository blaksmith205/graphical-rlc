function equation = equation_of(response)
syms C1 C2 sourceVal t s1 s2 a wd;
eq = 0;
if isequal(response, 'Over')
    eq = sourceVal + C1*exp(s1*t) + C2*exp(s2*t);
elseif isequal(response, 'Under')
    eq = sourceVal + exp(-a*t)*(C1*cos(wd*t)+C2*sin(wd*t));
elseif isequal(response, 'Critical')
    eq = sourceVal + C1*t*exp(-a*t)+C2*exp(-a*t);
end
equation = eq;
end    