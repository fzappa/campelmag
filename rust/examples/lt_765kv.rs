//! Example: 765kV Three-Phase Transmission Line
//!
//! Electric and Magnetic Field Profile Calculation
//!
//! Based on: EPRI AC - Transmission Line Reference Book - 200kV and above
//! Chapter 7 - Electric and Magnetic Fields
//!
//! Author: Alan Franco Rosa
//! Date: 03/08/2016
//! Rust implementation: 2025

use campelmag::CampelMag;
use ndarray::Array1;
use plotters::prelude::*;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n#### ELECTRIC AND MAGNETIC FIELD CALCULATION ####\n");

    let cm = CampelMag::new();

    // Line voltage (V)
    let v = 765e3;

    // Transmitted power (W)
    let p = 1.2e9;
    let ir = p / v;

    // Phase angles (degrees)
    let ang_a = 120.0;
    let ang_b = 0.0;
    let ang_c = -120.0;
    let angulos_abc = vec![ang_a, ang_b, ang_c];

    // Current with phase shift
    let i = cm.iri(ir, &angulos_abc);

    // Mechanical characteristics
    let linha_de_medicao = vec![-80.0, 80.0, 0.5, 1.5]; // [Xmin, Xmax, Step, Height]
    let hmax = 25.0; // [m]
    let flecha = 8.63; // [m]
    let espac_sub_cond = 0.457; // [m]
    let dcabo = 0.02959;
    let ncond = 4.0;

    let h0 = hmax - flecha - espac_sub_cond / 2.0;
    let h = vec![h0, h0, h0]; // Minimum cable height in bundle
    let d = vec![-14.34, 0.0, 14.34];

    println!("V = {:.2} [kV]", v / 1e3);
    println!("H = [{:.2}, {:.2}, {:.2}] [m]", h[0], h[1], h[2]);
    println!("D = [{:.2}, {:.2}, {:.2}] [m]", d[0], d[1], d[2]);
    println!(
        "Measurement profile = [{:.2}, {:.2}, {:.2}, {:.2}] [m]",
        linha_de_medicao[0], linha_de_medicao[1], linha_de_medicao[2], linha_de_medicao[3]
    );

    // Electric field calculation
    let ekvm = cm.calc_ekv(
        &h,
        &d,
        espac_sub_cond,
        ncond,
        dcabo,
        v,
        &angulos_abc,
        &linha_de_medicao,
    );

    // Magnetic field [SBr, SBx, SBh, B]
    let b = cm.brms(&i, &h, &d, &linha_de_medicao);

    // Find maximum values
    let e_max = ekvm.column(2).iter().cloned().fold(f64::NEG_INFINITY, f64::max);
    let b_max = b.column(3).iter().cloned().fold(f64::NEG_INFINITY, f64::max);

    println!("\nMaximum Electric Field = {:.2} [kV/m]", e_max);
    println!("Maximum Magnetic Field = {:.2} [µT]", b_max);

    // Generate position array for display
    let n_points = ((linha_de_medicao[1] - linha_de_medicao[0]) / linha_de_medicao[2]) as usize + 1;
    let mut px = Array1::<f64>::zeros(n_points);
    for idx in 0..n_points {
        px[idx] = linha_de_medicao[0] + (idx as f64) * linha_de_medicao[2];
    }

    // Display some sample points
    println!("\n--- Sample Results ---");
    println!("Position [m] | E-field [kV/m] | B-field [µT]");
    println!("-------------|----------------|-------------");

    for idx in (0..n_points).step_by(n_points / 10) {
        println!(
            "{:12.2} | {:14.4} | {:12.4}",
            px[idx], ekvm[[idx, 2]], b[[idx, 3]]
        );
    }

    // Plot Electric Field
    plot_electric_field(&px, &ekvm)?;

    // Plot Magnetic Field
    plot_magnetic_field(&px, &b)?;

    println!("\n✓ Calculation completed successfully!");
    println!("\n📊 Graphs generated:");
    println!("  - campo_eletrico.png (Electric Field)");
    println!("  - campo_magnetico.png (Magnetic Field)");

    Ok(())
}

fn plot_electric_field(px: &Array1<f64>, ekvm: &ndarray::Array2<f64>) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("campo_eletrico.png", (900, 700)).into_drawing_area();
    root.fill(&WHITE)?;

    let e_max = ekvm.column(2).iter().cloned().fold(f64::NEG_INFINITY, f64::max);

    // Tentar criar gráfico com texto, se falhar, criar sem texto
    match create_electric_chart_with_labels(&root, px, ekvm, e_max) {
        Ok(_) => {
            println!("✓ Electric field graph created with text labels");
        },
        Err(e) => {
            println!("⚠ Warning: Could not render text labels: {}", e);
            println!("  Creating graph without text (curves and grid only)...");

            // Fallback: criar gráfico simples sem texto
            let mut chart = ChartBuilder::on(&root)
                .margin(20)
                .build_cartesian_2d(px[0]..px[px.len() - 1], 0.0..(e_max * 1.1))?;

            chart.configure_mesh().draw()?;

            chart.draw_series(LineSeries::new(
                (0..px.len()).map(|i| (px[i], ekvm[[i, 2]])),
                RED.stroke_width(2),
            ))?;
        }
    }

    root.present()?;
    Ok(())
}

fn create_electric_chart_with_labels(
    root: &DrawingArea<BitMapBackend, plotters::coord::Shift>,
    px: &Array1<f64>,
    ekvm: &ndarray::Array2<f64>,
    e_max: f64,
) -> Result<(), Box<dyn std::error::Error>> {
    // Tentar usar FontDesc com ab_glyph - se funcionar ótimo, senão vamos para fallback
    let mut chart = ChartBuilder::on(root)
        .caption(
            "Campo Eletrico x Perfil de medicao",
            FontDesc::new(FontFamily::SansSerif, 24.0, FontStyle::Bold),
        )
        .margin(15)
        .x_label_area_size(45)
        .y_label_area_size(65)
        .build_cartesian_2d(px[0]..px[px.len() - 1], 0.0..(e_max * 1.1))?;

    chart
        .configure_mesh()
        .x_desc("Posicao na faixa de passagem [m]")
        .y_desc("Campo Eletrico [kV/m]")
        .axis_desc_style(FontDesc::new(FontFamily::SansSerif, 20.0, FontStyle::Normal))
        .label_style(FontDesc::new(FontFamily::SansSerif, 15.0, FontStyle::Normal))
        .draw()?;

    chart
        .draw_series(LineSeries::new(
            (0..px.len()).map(|i| (px[i], ekvm[[i, 2]])),
            RED.stroke_width(2),
        ))?
        .label("E_max")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], RED.stroke_width(2)));

    chart
        .configure_series_labels()
        .background_style(WHITE.mix(0.8))
        .border_style(BLACK)
        .label_font(FontDesc::new(FontFamily::SansSerif, 16.0, FontStyle::Normal))
        .draw()?;

    Ok(())
}

fn plot_magnetic_field(px: &Array1<f64>, b: &ndarray::Array2<f64>) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("campo_magnetico.png", (900, 700)).into_drawing_area();
    root.fill(&WHITE)?;

    let b_max = b.column(3).iter().cloned().fold(f64::NEG_INFINITY, f64::max);

    // Tentar criar gráfico com texto, se falhar, criar sem texto
    match create_magnetic_chart_with_labels(&root, px, b, b_max) {
        Ok(_) => {
            println!("✓ Magnetic field graph created with text labels");
        },
        Err(e) => {
            println!("⚠ Warning: Could not render text labels: {}", e);
            println!("  Creating graph without text (curves and grid only)...");

            // Fallback: criar gráfico simples sem texto
            let mut chart = ChartBuilder::on(&root)
                .margin(20)
                .build_cartesian_2d(px[0]..px[px.len() - 1], 0.0..(b_max * 1.1))?;

            chart.configure_mesh().draw()?;

            chart.draw_series(LineSeries::new(
                (0..px.len()).map(|i| (px[i], b[[i, 3]])),
                BLUE.stroke_width(2),
            ))?;
        }
    }

    root.present()?;
    Ok(())
}

fn create_magnetic_chart_with_labels(
    root: &DrawingArea<BitMapBackend, plotters::coord::Shift>,
    px: &Array1<f64>,
    b: &ndarray::Array2<f64>,
    b_max: f64,
) -> Result<(), Box<dyn std::error::Error>> {
    // Tentar usar FontDesc com ab_glyph - se funcionar ótimo, senão vamos para fallback
    let mut chart = ChartBuilder::on(root)
        .caption(
            "Campo Magnetico x Perfil de medicao",
            FontDesc::new(FontFamily::SansSerif, 24.0, FontStyle::Bold),
        )
        .margin(15)
        .x_label_area_size(45)
        .y_label_area_size(65)
        .build_cartesian_2d(px[0]..px[px.len() - 1], 0.0..(b_max * 1.1))?;

    chart
        .configure_mesh()
        .x_desc("Posicao na faixa de passagem [m]")
        .y_desc("Campo Magnetico [uT]")
        .axis_desc_style(FontDesc::new(FontFamily::SansSerif, 20.0, FontStyle::Normal))
        .label_style(FontDesc::new(FontFamily::SansSerif, 15.0, FontStyle::Normal))
        .draw()?;

    chart
        .draw_series(LineSeries::new(
            (0..px.len()).map(|i| (px[i], b[[i, 3]])),
            BLUE.stroke_width(2),
        ))?
        .label("B_mag")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], BLUE.stroke_width(2)));

    chart
        .configure_series_labels()
        .background_style(WHITE.mix(0.8))
        .border_style(BLACK)
        .label_font(FontDesc::new(FontFamily::SansSerif, 16.0, FontStyle::Normal))
        .draw()?;

    Ok(())
}
