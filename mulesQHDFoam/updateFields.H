/*
 *
 * Gradients at faces
 *
 */
gradUf = fvsc::grad(U);

gradWf = fvsc::grad(W);

gradTf = fvsc::grad(T);

/*
 *
 * QGD coefficients now updated in thermo library
 *
 */
muf    = linearInterpolate(turbulence->mu());
alphaf = linearInterpolate(turbulence->alpha());

/*
 *
 * Linear interpolation of fields from volumes to face centers
 *
 */

// Density
rhof = linearInterpolate(rho);

// Velocity
Uf = linearInterpolate(U);
    
// T

Tf = linearInterpolate(T);

BdFrc  = -beta*(T-T0)*g;
BdFrcf = fvc::interpolate(BdFrc);

//Hi
Hif = alphaf/rhof;

// Heat capacity at constant pressure
Cpf = linearInterpolate(thermo.Cp());




