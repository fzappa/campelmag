# CampelMag - Rust Edition

**Electric and Magnetic Fields Calculation for Transmission Lines**

[![Rust](https://img.shields.io/badge/rust-1.70%2B-orange.svg)](https://www.rust-lang.org/)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Implementação em Rust para cálculo 2D de campos elétricos e magnéticos de linhas de transmissão aéreas.

Baseado em: **EPRI AC - Transmission Line Reference Book - 200kV and above**
Capítulo 7 - Electric and Magnetic Fields

Autor: Alan Franco Rosa (2016)
Implementação Rust: 2025

---

## 🚀 Por que Rust?

Esta implementação em Rust oferece:

- ✅ **Performance**: Velocidade comparável a C/C++
- ✅ **Segurança**: Sistema de tipos forte e memory safety
- ✅ **Concorrência**: Safe concurrency sem data races
- ✅ **Zero-cost abstractions**: Alta abstração sem overhead
- ✅ **Ferramentas modernas**: Cargo, rustfmt, clippy
- ✅ **Documentação**: Documentação integrada com `cargo doc`

---

## 📋 Requisitos

- Rust 1.70 ou superior
- Cargo (geralmente vem com Rust)

### Instalação do Rust

#### Windows

```powershell
# Baixe e execute o instalador
# https://www.rust-lang.org/tools/install
winget install Rustlang.Rustup
```

#### Linux/macOS

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

Verifique a instalação:

```bash
rustc --version
cargo --version
```

---

## 🔧 Instalação e Uso

### 1. Compilar o projeto

```bash
cd rust
cargo build --release
```

### 2. Executar os testes

```bash
cargo test
```

### 3. Executar o exemplo (765kV)

**Apenas cálculos (sem gráficos):**

```bash
cargo run --example lt_765kv
```

**Com geração de gráficos:**

```bash
cargo run --example lt_765kv --features plotting
```

Isso gerará os arquivos `campo_eletrico.png` e `campo_magnetico.png` no diretório atual.

> **Nota sobre os gráficos:** O código tenta gerar gráficos com texto completo (título, labels, legendas). Se as fontes do sistema não estiverem disponíveis (comum em ambientes WSL/Linux sem configuração de fontes), gráficos simplificados sem texto serão gerados automaticamente. Os gráficos mostram as curvas vermelha (campo elétrico) e azul (campo magnético) com grid de referência.

### 4. Gerar documentação

```bash
cargo doc --open
```

---

## 📚 Uso como Biblioteca

Adicione ao seu `Cargo.toml`:

```toml
[dependencies]
campelmag = { path = "../campelmag/rust" }
```

### Exemplo básico

```rust
use campelmag::CampelMag;

fn main() {
    let cm = CampelMag::new();

    // Configuração da linha
    let v = 765e3; // Tensão (V)
    let h = vec![16.37, 16.37, 16.37]; // Altura dos condutores (m)
    let d = vec![-14.34, 0.0, 14.34]; // Distância entre condutores (m)
    let angulos_abc = vec![120.0, 0.0, -120.0]; // Ângulos das fases (graus)

    // Parâmetros do feixe
    let espac_sub_cond = 0.457; // Espaçamento entre subcondutores (m)
    let ncond = 4.0; // Número de condutores por feixe
    let dcabo = 0.02959; // Diâmetro do cabo (m)

    // Linha de medição [Xmin, Xmax, Passo, Altura]
    let linha_de_medicao = vec![-80.0, 80.0, 0.5, 1.5];

    // Cálculo do campo elétrico
    let e = cm.calc_ekv(
        &h,
        &d,
        espac_sub_cond,
        ncond,
        dcabo,
        v,
        &angulos_abc,
        &linha_de_medicao,
    );

    println!("Campo elétrico calculado: {} pontos", e.nrows());
}
```

---

## 🎯 Funcionalidades

A struct `CampelMag` oferece métodos para:

### Campos Elétricos e Magnéticos

- `calc_ekv()` - Cálculo de campo elétrico
- `brms()` - Cálculo de campo magnético
- `erms()` - Campo elétrico RMS

### Coeficientes de Potencial

- `pkl()` - Coeficiente de potencial mútuo
- `pkk()` - Coeficiente de potencial próprio
- `matriz_p()` - Matriz de coeficientes de Maxwell

### Efeito Corona

- `corona_vc()` - Tensão crítica (Equação de Peek)
- `corona_delta()` - Delta crítico
- `corona_perdas()` - Perdas por corona
- `corona_ec()` - Campo elétrico crítico

### Funções Auxiliares

- `vri()` - Matriz de tensões
- `iri()` - Matriz de correntes
- `deq()` - Diâmetro equivalente do feixe
- `db()` - Diâmetro do feixe
- `skl()` - Distância entre condutores
- `skll()` - Distância entre condutor e imagem
- `prof_img()` - Profundidade da imagem

---

## 📊 Estrutura do Projeto

```
rust/
├── Cargo.toml              # Configuração do projeto
├── src/
│   └── lib.rs              # Biblioteca principal
├── examples/
│   └── lt_765kv.rs         # Exemplo: Linha de 765kV (com gráficos)
└── README.md               # Este arquivo
```

## 📈 Visualização de Resultados

O exemplo `lt_765kv` pode gerar gráficos PNG dos campos elétricos e magnéticos:

```bash
cargo run --example lt_765kv --features plotting
```

**Arquivos gerados:**

- `campo_eletrico.png` - Perfil do campo elétrico (curva vermelha)
- `campo_magnetico.png` - Perfil do campo magnético (curva azul)

Os gráficos incluem grid e eixos para facilitar a leitura dos valores.

---

## 🧪 Testes

Execute os testes unitários:

```bash
cargo test
```

Execute com output detalhado:

```bash
cargo test -- --nocapture
```

---

## 🔍 Comparação com Outras Linguagens

| Característica | Python     | Julia    | C++        | **Rust**   |
| -------------- | ---------- | -------- | ---------- | ---------- |
| Performance    | ⭐⭐       | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Memory Safety  | ⭐⭐⭐     | ⭐⭐⭐   | ⭐⭐       | ⭐⭐⭐⭐⭐ |
| Facilidade     | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐       | ⭐⭐⭐     |
| Concorrência   | ⭐⭐⭐     | ⭐⭐⭐⭐ | ⭐⭐⭐     | ⭐⭐⭐⭐⭐ |
| Ecossistema    | ⭐⭐⭐⭐⭐ | ⭐⭐⭐   | ⭐⭐⭐⭐   | ⭐⭐⭐⭐   |

---

## 🛠️ Desenvolvimento

### Formatar código

```bash
cargo fmt
```

### Linter (clippy)

```bash
cargo clippy
```

### Benchmark (futuro)

```bash
cargo bench
```

---

## 📖 Documentação Completa

Gere a documentação HTML:

```bash
cargo doc --open
```

Isso abrirá automaticamente a documentação no navegador.

---

## 🎓 Aprendizado de Rust

Esta implementação é excelente para estudar:

1. **Ownership e Borrowing**: Sistema único de gerenciamento de memória
2. **Traits**: Polimorfismo tipo-seguro
3. **Arrays multidimensionais**: Uso da biblioteca `ndarray`
4. **Números complexos**: Biblioteca `num-complex`
5. **Documentação**: Comentários `///` e `//!`
6. **Testes**: Framework de testes integrado
7. **Módulos**: Organização de código em Rust

---

## 🌟 Próximos Passos

Ideias para melhorias futuras:

- [x] Adicionar suporte a gráficos com `plotters` ✅
- [ ] Melhorar gráficos com fontes embutidas (título, labels, legendas)
- [ ] Implementar cálculos paralelos com `rayon`
- [ ] Criar interface CLI com `clap`
- [ ] Adicionar serialização com `serde`
- [ ] Criar binding para Python (PyO3)
- [ ] Implementar benchmark comparativo
- [ ] Adicionar mais exemplos de uso
- [ ] Exportar dados para CSV

---

## 📄 Licença

Veja o arquivo [LICENSE](../LICENSE) para mais detalhes.

---

## 👤 Autor

**Alan Franco Rosa**
Implementação original: 2016
Versão Rust: 2025

---

## 🤝 Contribuindo

Contribuições são bem-vindas! Sinta-se à vontade para:

1. Fork o projeto
2. Criar uma branch (`git checkout -b feature/nova-funcionalidade`)
3. Commit suas mudanças (`git commit -m 'Adiciona nova funcionalidade'`)
4. Push para a branch (`git push origin feature/nova-funcionalidade`)
5. Abrir um Pull Request

---

## 📚 Referências

- **EPRI AC Transmission Line Reference Book** - 200kV and above, Chapter 7
- [The Rust Programming Language Book](https://doc.rust-lang.org/book/)
- [ndarray Documentation](https://docs.rs/ndarray/)
- [num-complex Documentation](https://docs.rs/num-complex/)

---

**Happy Coding! 🦀**
