function response = determine_response(roots)
if size(roots) ~= 4
    response = '';
else
    if roots(1) == roots(2)
        response = 'Critical';
    elseif roots(3) ~= 0
        response = 'Under';
    else
        response = 'Over';
    end
end
end