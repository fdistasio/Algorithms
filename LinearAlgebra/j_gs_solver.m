function [x_new] = j_gs_solver(A, b, x_old, tol, max_iter)

    err = +inf;
    i = 1;

    while(err > tol && i <= max_iter)
        x_new = my_jacobi(A, b, x_old); % x_new = my_gauss_seidel(A, b, x_old);
        err = norm(x_new - x_old, inf);
        x_old = x_new;
        i = i + 1;
    end
    
end