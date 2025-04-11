function [x_new, k, status] = fixed_point(x_old, g, tol, max_iter)

    err = +inf;
    k = 0;

    while(err >= tol && k < max_iter)

        x_new = g(x_old);
        err = abs(x_new - x_old);
        x_old = x_new;
        k = k + 1;

    end

    if(err < tol)
        status = "Convergent";
    else
        status = "Divergent";
    end

end
