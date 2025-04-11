function [x_new, k, status] = newton_method(x_old, f, f_prime, tol, max_iter)

    err = +inf;
    k = 0;

    while(err >= tol && k < max_iter)

        f_prime_val = f_prime(x_old);

        if(f_prime_val == 0)
            status = "Divergent";
            return;
        end

        x_new = x_old - f(x_old)/f_prime_val;
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