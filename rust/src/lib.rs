//! # CampelMag
//!
//! Electric and Magnetic Fields Calculation for Transmission Lines
//!
//! Based on: **EPRI AC - Transmission Line Reference Book - 200kV and above**
//! Chapter 7 - Electric and Magnetic Fields
//!
//! Author: Alan Franco Rosa (2016)
//! Rust implementation: 2025
//!
//! ## Example
//!
//! ```
//! use campelmag::CampelMag;
//!
//! let cm = CampelMag::new();
//! let v = 765e3; // Voltage [V]
//! let h = vec![16.37, 16.37, 16.37]; // Heights [m]
//! let d = vec![-14.34, 0.0, 14.34]; // Distances [m]
//! let angles = vec![120.0, 0.0, -120.0]; // Phase angles [degrees]
//! ```

use ndarray::{Array1, Array2};
use num_complex::Complex64;
use std::f64::consts::PI;

/// Permittivity of free space (ε₀) [F/m]
const EPSILON_0: f64 = 8.854187817e-12;

/// Square root of 3
const SQRT_3: f64 = 1.7320508075688772;

/// Main struct for electric and magnetic field calculations
#[derive(Debug, Clone)]
pub struct CampelMag;

impl CampelMag {
    /// Creates a new instance of CampelMag
    pub fn new() -> Self {
        CampelMag
    }

    /// Returns the voltage matrix (3x2) for each phase [real, imaginary]
    ///
    /// # Arguments
    ///
    /// * `v` - Phase voltage [V]
    /// * `angles` - Phase angles [A, B, C] in degrees
    ///
    /// # Returns
    ///
    /// Matrix (3x2) with voltages for each phase
    pub fn vri(&self, v: f64, angles: &[f64]) -> Array2<f64> {
        let mut result = Array2::<f64>::zeros((3, 2));

        for (i, &angle) in angles.iter().enumerate() {
            let ang_rad = angle.to_radians();
            result[[i, 0]] = ang_rad.cos() * v / SQRT_3;
            result[[i, 1]] = ang_rad.sin() * v / SQRT_3;
        }

        result
    }

    /// Calculates distance between conductor k and image of conductor l (3x3)
    ///
    /// # Arguments
    ///
    /// * `d` - Relative position of phases [m]
    /// * `h` - Relative height of phases [m]
    ///
    /// # Returns
    ///
    /// Matrix (3x3) with distances
    pub fn skll(&self, d: &[f64], h: &[f64]) -> Array2<f64> {
        let mut result = Array2::<f64>::zeros((3, 3));

        result[[0, 0]] = 2.0 * h[0];
        result[[0, 1]] = ((d[1] - d[0]).powi(2) + (2.0 * h[1]).powi(2)).sqrt();
        result[[0, 2]] = ((d[2] - d[0]).powi(2) + (2.0 * h[2]).powi(2)).sqrt();

        result[[1, 0]] = ((d[0] - d[1]).powi(2) + (2.0 * h[0]).powi(2)).sqrt();
        result[[1, 1]] = 2.0 * h[1];
        result[[1, 2]] = ((d[2] - d[1]).powi(2) + (2.0 * h[2]).powi(2)).sqrt();

        result[[2, 0]] = ((d[0] - d[2]).powi(2) + (2.0 * h[0]).powi(2)).sqrt();
        result[[2, 1]] = ((d[1] - d[2]).powi(2) + (2.0 * h[1]).powi(2)).sqrt();
        result[[2, 2]] = 2.0 * h[2];

        result
    }

    /// Calculates distance between conductors k and l (3x3)
    ///
    /// # Arguments
    ///
    /// * `d` - Relative position of phases [m]
    /// * `h` - Relative height of phases [m]
    ///
    /// # Returns
    ///
    /// Matrix (3x3) with distances between conductors
    pub fn skl(&self, d: &[f64], h: &[f64]) -> Array2<f64> {
        let mut result = Array2::<f64>::zeros((3, 3));

        result[[0, 0]] = 0.0;
        result[[0, 1]] = ((d[1] - d[0]).powi(2) + (h[1] - h[0]).powi(2)).sqrt();
        result[[0, 2]] = ((d[2] - d[0]).powi(2) + (h[2] - h[0]).powi(2)).sqrt();

        result[[1, 0]] = ((d[0] - d[1]).powi(2) + (h[0] - h[1]).powi(2)).sqrt();
        result[[1, 1]] = 0.0;
        result[[1, 2]] = ((d[2] - d[1]).powi(2) + (h[2] - h[1]).powi(2)).sqrt();

        result[[2, 0]] = ((d[0] - d[2]).powi(2) + (h[0] - h[2]).powi(2)).sqrt();
        result[[2, 1]] = ((d[1] - d[2]).powi(2) + (h[1] - h[2]).powi(2)).sqrt();
        result[[2, 2]] = 0.0;

        result
    }

    /// Calculates mutual potential coefficient between conductors k and l (3x3)
    ///
    /// # Arguments
    ///
    /// * `proprio` - Distance between conductor k and image of conductor l
    /// * `mutuo` - Distance between conductor k and l
    ///
    /// # Returns
    ///
    /// Matrix (3x3) with mutual potential coefficients
    pub fn pkl(&self, proprio: &Array2<f64>, mutuo: &Array2<f64>) -> Array2<f64> {
        let factor = 1.0 / (2.0 * PI * EPSILON_0);
        (proprio / mutuo).mapv(|x| factor * x.ln())
    }

    /// Calculates self potential coefficient of conductor k
    ///
    /// # Arguments
    ///
    /// * `h` - Conductor height [m]
    /// * `deq` - Equivalent conductor diameter [m]
    ///
    /// # Returns
    ///
    /// Array with self potential coefficients
    pub fn pkk(&self, h: &[f64], deq: f64) -> Array1<f64> {
        let factor = 1.0 / (2.0 * PI * EPSILON_0);
        Array1::from_vec(h.iter().map(|&hi| factor * (4.0 * hi / deq).ln()).collect())
    }

    /// Returns the current matrix (3x1) [complex]
    ///
    /// # Arguments
    ///
    /// * `i` - Current per phase (balanced circuit) [A]
    /// * `angles` - Phase angles [A, B, C] in degrees
    ///
    /// # Returns
    ///
    /// Array (3) with complex currents
    pub fn iri(&self, i: f64, angles: &[f64]) -> Array1<Complex64> {
        Array1::from_vec(
            angles
                .iter()
                .map(|&angle| {
                    let ang_rad = angle.to_radians();
                    Complex64::new(
                        ang_rad.cos() * i / SQRT_3,
                        ang_rad.sin() * i / SQRT_3,
                    )
                })
                .collect(),
        )
    }

    /// Calculates equivalent bundle diameter
    ///
    /// # Arguments
    ///
    /// * `db` - Bundle diameter [m]
    /// * `dcable` - Cable diameter [m]
    /// * `n` - Number of conductors
    ///
    /// # Returns
    ///
    /// Equivalent bundle diameter [m]
    pub fn deq(&self, db: f64, dcable: f64, n: f64) -> f64 {
        db * (n * dcable / db).powf(1.0 / n)
    }

    /// Calculates bundle diameter
    ///
    /// # Arguments
    ///
    /// * `s` - Spacing between conductors [m]
    /// * `n` - Number of conductors
    ///
    /// # Returns
    ///
    /// Bundle diameter [m]
    pub fn db(&self, s: f64, n: f64) -> f64 {
        s / (PI / n).sin()
    }

    /// Returns the image depth for the magnetic field
    ///
    /// # Arguments
    ///
    /// * `r` - Ground resistivity ρ [Ω]
    /// * `p` - Ground permeability μ [H/m] - Typical = 4π × 10⁻⁷ H/m
    /// * `f` - Network frequency [Hz]
    ///
    /// # Returns
    ///
    /// Depth in meters [Δ]
    pub fn prof_img(&self, r: f64, p: f64, f: f64) -> f64 {
        (r / (PI * f * p)).sqrt()
    }

    /// Returns Maxwell's potential coefficient matrix (3x3)
    ///
    /// # Arguments
    ///
    /// * `mp` - Self coefficient matrix
    /// * `mm` - Mutual coefficient matrix
    ///
    /// # Returns
    ///
    /// Matrix (3x3) of potential coefficients
    pub fn matriz_p(&self, mp: &Array1<f64>, mm: &Array2<f64>) -> Array2<f64> {
        let mut m = mm.clone();
        m[[0, 0]] = mp[0];
        m[[1, 1]] = mp[1];
        m[[2, 2]] = mp[2];
        m
    }

    /// Calculates critical peak voltage (Peek's equation)
    ///
    /// # Arguments
    ///
    /// * `m` - Roughness coefficient (0.93 for wires, 0.87 for cables)
    /// * `delta` - Coefficient depending on height and altitude
    /// * `dcond` - Conductor diameter [m]
    /// * `dist_cond` - Distance between conductors [m]
    ///
    /// # Returns
    ///
    /// Critical voltage [kV]
    pub fn corona_vc(&self, m: f64, delta: f64, dcond: f64, dist_cond: f64) -> f64 {
        let a0 = 2.43 * m * delta * dcond * 1000.0;
        let a1 = ((2.0 * dist_cond * 1000.0) / (dcond * 1000.0)).log10();
        a0 * a1
    }

    /// Calculates critical delta
    ///
    /// # Arguments
    ///
    /// * `h` - Installation altitude [m]
    /// * `t` - Average annual temperature [°C]
    ///
    /// # Returns
    ///
    /// Critical delta
    pub fn corona_delta(&self, h: f64, t: f64) -> f64 {
        (0.386 * (760.0 - 0.086 * h)) / (273.0 + t)
    }

    /// Calculates losses due to corona effect
    ///
    /// # Arguments
    ///
    /// * `delta` - Critical delta
    /// * `f` - Network frequency [Hz]
    /// * `dcond` - Conductor diameter [m]
    /// * `dist_cond` - Distance between conductors [m]
    /// * `v` - Peak-to-peak network voltage [V]
    /// * `vc` - Critical line voltage [V]
    ///
    /// # Returns
    ///
    /// Losses [kW/km]
    pub fn corona_perdas(&self, delta: f64, f: f64, dcond: f64, dist_cond: f64, v: f64, vc: f64) -> f64 {
        let a0 = (3.44 / delta) * f;
        let a1 = ((dcond * 1000.0) / (2.0 * (dist_cond * 1000.0))).sqrt();
        let a2 = (v - vc).powi(2) * 0.001;
        a0 * a1 * a2
    }

    /// Calculates critical electric field
    ///
    /// # Arguments
    ///
    /// * `m` - Roughness coefficient (0.93 for wires, 0.87 for cables)
    /// * `delta` - Critical delta
    /// * `dcond` - Conductor diameter [m]
    ///
    /// # Returns
    ///
    /// Critical electric field [kV/cm]
    pub fn corona_ec(&self, m: f64, delta: f64, dcond: f64) -> f64 {
        let a0 = m * 21.1e3 * delta;
        let a1 = 1.0 + ((1.0_f64 / 0.301).sqrt() / (delta * dcond * 1000.0).sqrt());
        a0 * a1
    }

    /// Calculates charge distribution matrix
    ///
    /// # Arguments
    ///
    /// * `h` - Conductor heights [HA, HB, HC] [m]
    /// * `d` - Distances between conductors [DA, DB, DC] [m]
    /// * `espac_sub_cond` - Spacing between subconductors [m]
    /// * `ncond` - Number of conductors per bundle
    /// * `dcabo` - Bundle cable diameter [m]
    /// * `v` - Line voltage [V]
    /// * `angulos_abc` - Angular phase shift [AA, AB, AC] [degrees]
    ///
    /// # Returns
    ///
    /// Charge distribution matrix Q
    pub fn calc_q(
        &self,
        h: &[f64],
        d: &[f64],
        espac_sub_cond: f64,
        ncond: f64,
        dcabo: f64,
        v: f64,
        angulos_abc: &[f64],
    ) -> Array2<f64> {
        let db = self.db(espac_sub_cond, ncond);
        let deq = self.deq(db, dcabo, ncond);

        let mp = self.pkk(h, deq);
        let mm = self.pkl(&self.skll(d, h), &self.skl(d, h));

        let p = self.matriz_p(&mp, &mm);
        let c = invert_3x3(&p);
        let v_matrix = self.vri(v, angulos_abc);

        c.dot(&v_matrix)
    }

    /// Returns a vector with electric fields at measurement points [SEx, SEy, Erms]
    ///
    /// # Arguments
    ///
    /// * `linha_de_med` - Measurement line characteristics [-X, X, Step, Height]
    /// * `h` - Vertical positions of cables [A, B, C] [m]
    /// * `d` - Relative horizontal positions of cables [A, 0, C] [m]
    /// * `q` - Charge matrix
    ///
    /// # Returns
    ///
    /// Matrix (N, 3) with [SEx, SEy, Erms]
    pub fn erms(
        &self,
        linha_de_med: &[f64],
        h: &[f64],
        d: &[f64],
        q: &Array2<f64>,
    ) -> Array2<f64> {
        let n_points = ((linha_de_med[1] - linha_de_med[0]) / linha_de_med[2]) as usize + 1;
        let mut px = Array1::<f64>::zeros(n_points);
        for i in 0..n_points {
            px[i] = linha_de_med[0] + (i as f64) * linha_de_med[2];
        }

        let mut ex = Array2::<Complex64>::zeros((n_points, 3));
        let mut ey = Array2::<Complex64>::zeros((n_points, 3));
        let hm = linha_de_med[3];

        for i in 0..3 {
            let xm = &px - d[i];
            let hk = h[i];

            let c = Complex64::new(q[[i, 0]], q[[i, 1]]) / (2.0 * PI * EPSILON_0);

            for j in 0..n_points {
                let t1x = xm[j] / (xm[j].powi(2) + (hk - hm).powi(2));
                let t2x = xm[j] / (xm[j].powi(2) + (hk + hm).powi(2));

                let t1y = (hm - hk) / (xm[j].powi(2) + (hk - hm).powi(2));
                let t2y = (hm + hk) / (xm[j].powi(2) + (hk + hm).powi(2));

                ex[[j, i]] = c * (t1x - t2x);
                ey[[j, i]] = c * (t1y - t2y);
            }
        }

        let mut result = Array2::<f64>::zeros((n_points, 3));
        for i in 0..n_points {
            let sex: f64 = ex.row(i).iter().map(|c| c.re).sum();
            let sey: f64 = ey.row(i).iter().map(|c| c.re).sum();
            result[[i, 0]] = sex;
            result[[i, 1]] = sey;
            result[[i, 2]] = (sex.powi(2) + sey.powi(2)).sqrt();
        }

        result
    }

    /// Calculates electric field measured in a profile
    ///
    /// # Arguments
    ///
    /// * `h` - Conductor heights [HA, HB, HC] [m]
    /// * `d` - Distances between conductors [DA, DB, DC] [m]
    /// * `espac_sub_cond` - Spacing between subconductors [m]
    /// * `ncond` - Number of conductors per bundle
    /// * `dcabo` - Bundle cable diameter [m]
    /// * `v` - Line voltage [V]
    /// * `angulos_abc` - Angular phase shift [AA, AB, AC] [degrees]
    /// * `linha_de_medicao` - Measurement profile [Xmin, Xmax, Step, Height] [m]
    ///
    /// # Returns
    ///
    /// Matrix with electric fields in [kV/m]
    pub fn calc_ekv(
        &self,
        h: &[f64],
        d: &[f64],
        espac_sub_cond: f64,
        ncond: f64,
        dcabo: f64,
        v: f64,
        angulos_abc: &[f64],
        linha_de_medicao: &[f64],
    ) -> Array2<f64> {
        let q = self.calc_q(h, d, espac_sub_cond, ncond, dcabo, v, angulos_abc);
        let result = self.erms(linha_de_medicao, h, d, &q);
        result / 1e3
    }

    /// Returns a vector with resulting magnetic field at right-of-way points
    ///
    /// # Arguments
    ///
    /// * `i` - Current passing through the line [IA, IB, IC] [A]
    /// * `h` - Vertical positions of cables [A, B, C] [m]
    /// * `d` - Relative horizontal positions of cables [A, 0, C] [m]
    /// * `p` - Measurement line characteristics [-X, X, Step, Height]
    ///
    /// # Returns
    ///
    /// Matrix (N, 4) with [SBr, SBx, SBh, B] in micro Tesla (µT)
    pub fn brms(
        &self,
        i: &Array1<Complex64>,
        h: &[f64],
        d: &[f64],
        p: &[f64],
    ) -> Array2<f64> {
        let n_points = ((p[1] - p[0]) / p[2]) as usize + 1;
        let mut xm = Array1::<f64>::zeros(n_points);
        for idx in 0..n_points {
            xm[idx] = p[0] + (idx as f64) * p[2];
        }

        let hm = p[3];
        let mut bkr = Array2::<Complex64>::zeros((n_points, 3));
        let mut bkx = Array2::<Complex64>::zeros((n_points, 3));
        let mut bkh = Array2::<Complex64>::zeros((n_points, 3));

        for idx in 0..3 {
            for j in 0..n_points {
                let dist_sq = (xm[j] - d[idx]).powi(2) + (hm - h[idx]).powi(2);
                let dist = dist_sq.sqrt();

                bkr[[j, idx]] = (2e-7 * i[idx]) / dist;
                bkx[[j, idx]] = (2e-7 * i[idx] * (xm[j] - d[idx])) / dist;
                bkh[[j, idx]] = (2e-7 * i[idx] * (hm - h[idx])) / dist;
            }
        }

        let mut result = Array2::<f64>::zeros((n_points, 4));
        for i in 0..n_points {
            let sbr: f64 = bkr.row(i).iter().map(|c| c.re).sum();
            let sbx: f64 = bkx.row(i).iter().map(|c| c.re).sum();
            let sbh: f64 = bkh.row(i).iter().map(|c| c.re).sum();
            let bmag = (sbx.powi(2) + sbh.powi(2)).sqrt();

            result[[i, 0]] = sbr * 1e6;
            result[[i, 1]] = sbx * 1e6;
            result[[i, 2]] = sbh * 1e6;
            result[[i, 3]] = bmag * 1e6;
        }

        result
    }
}

impl Default for CampelMag {
    fn default() -> Self {
        Self::new()
    }
}

/// Helper function to invert a 3x3 matrix
fn invert_3x3(matrix: &Array2<f64>) -> Array2<f64> {
    let a = matrix[[0, 0]];
    let b = matrix[[0, 1]];
    let c = matrix[[0, 2]];
    let d = matrix[[1, 0]];
    let e = matrix[[1, 1]];
    let f = matrix[[1, 2]];
    let g = matrix[[2, 0]];
    let h = matrix[[2, 1]];
    let i = matrix[[2, 2]];

    let det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

    let mut inv = Array2::<f64>::zeros((3, 3));
    inv[[0, 0]] = (e * i - f * h) / det;
    inv[[0, 1]] = (c * h - b * i) / det;
    inv[[0, 2]] = (b * f - c * e) / det;
    inv[[1, 0]] = (f * g - d * i) / det;
    inv[[1, 1]] = (a * i - c * g) / det;
    inv[[1, 2]] = (c * d - a * f) / det;
    inv[[2, 0]] = (d * h - e * g) / det;
    inv[[2, 1]] = (b * g - a * h) / det;
    inv[[2, 2]] = (a * e - b * d) / det;

    inv
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_db_calculation() {
        let cm = CampelMag::new();
        let db = cm.db(0.457, 4.0);
        assert!((db - 0.6459).abs() < 0.001);
    }

    #[test]
    fn test_deq_calculation() {
        let cm = CampelMag::new();
        let db = cm.db(0.457, 4.0);
        let deq = cm.deq(db, 0.02959, 4.0);
        assert!((deq - 0.4228).abs() < 0.001);
    }

    #[test]
    fn test_corona_delta() {
        let cm = CampelMag::new();
        let delta = cm.corona_delta(0.0, 25.0);
        assert!((delta - 0.986).abs() < 0.01);
    }
}
