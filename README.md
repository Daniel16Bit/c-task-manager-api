# 🗂️ C Task Manager API
## 🇧🇷 Descrição

> API REST para gerenciamento de tarefas construída **do zero** em C puro, sem frameworks.

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_%28programming_language%29)
[![Platform: Windows](https://img.shields.io/badge/Platform-Windows-green.svg)](https://www.microsoft.com/windows)
[![Database: SQLite](https://img.shields.io/badge/Database-SQLite-lightgrey.svg)](https://www.sqlite.org/)

---

## 📌 Sobre o Projeto

Este projeto implementa uma **API REST completa** em C, incluindo:

* ✅ Servidor HTTP construído manualmente com **Winsock** (sockets TCP/IP)
* ✅ Parser HTTP para processar requisições
* ✅ Sistema de roteamento para múltiplos endpoints
* ✅ Persistência de dados com **SQLite**
* ✅ Suporte completo a **CRUD** (Create, Read, Update, Delete)

**Objetivo:** Solidificar conhecimentos de programação de baixo nível, networking e arquitetura de APIs.

---

## 🛠️ Tecnologias Utilizadas

* **Linguagem:** C (C11)
* **Networking:** Winsock2 (Windows Sockets API)
* **Banco de Dados:** SQLite3
* **Protocolo:** HTTP/1.1
* **Formato de Dados:** JSON

---

## 📦 Instalação e Dependências

### **Requisitos de Sistema:**

* **Sistema Operacional:** Windows 10/11 (64-bit)
* **Compilador:** GCC (através do MinGW-w64 ou MSYS2)
* **Bibliotecas:** Winsock2 (inclusa no Windows), SQLite3 (incluída no projeto)

### **1. Instalar o Compilador GCC**

#### **Opção A: MSYS2 (Recomendado)**

1. Baixe o instalador: [https://www.msys2.org/](https://www.msys2.org/)
2. Instale e execute o MSYS2 UCRT64
3. Atualize os pacotes:

```bash
pacman -Syu
```

4. Instale o GCC:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

5. Adicione ao PATH:

```
C:\msys64\ucrt64\bin
```

#### **Opção B: MinGW-w64**

1. Baixe em: [https://www.mingw-w64.org/](https://www.mingw-w64.org/)
2. Instale e adicione ao PATH do sistema

### **2. Dependências Incluídas no Projeto**

O projeto **já inclui todas as dependências necessárias**:

```
project/
└── sqlite/
    ├── sqlite3.h
    └── sqlite3.c
```

**SQLite está embutido no projeto.** Não é necessário instalar separadamente.

### **3. Bibliotecas do Sistema (Windows)**

* **Winsock2** (`ws2_32.dll`) – incluída no Windows
* Linkada na compilação com `-lws2_32`

### **4. Verificar Instalação**

```bash
gcc --version
```

---

## 🏗️ Arquitetura Detalhada

### **Estrutura Completa de Arquivos:**

```
c-task-manager-api/
├── project/
│   ├── main.c
│   ├── server.h / server.c
│   ├── http.h / http.c
│   ├── json_parser.h / json_parser.c
│   ├── cli.h / cli.c
│   ├── tests.h / tests.c
│   ├── database.h / database.c
│   ├── sqlite/
│   │   ├── sqlite3.h
│   │   └── sqlite3.c
│   └── tasks.db
├── screenshots/
├── README.md
└── .gitignore
```

### **📂 Descrição dos Módulos:**

#### **🎯 main.c – Ponto de Entrada**

Responsável pela inicialização da aplicação, menu interativo e seleção de modo de execução.

#### **🌐 server.c – Camada de Rede**

Gerencia sockets TCP/IP, aceita conexões e delega requisições ao módulo HTTP.

#### **📡 http.c – Processamento HTTP**

Parseia requisições, roteia endpoints e monta respostas HTTP/JSON.

#### **📋 json_parser.c – Parser JSON**

Extrai dados simples de tarefas a partir de JSON bruto.

#### **💾 database.c – Persistência**

Abstrai operações CRUD usando SQLite embutido.

#### **🖥️ cli.c – Interface CLI**

Menu interativo para gerenciamento de tarefas via terminal.

#### **🧪 tests.c – Testes Automatizados**

Valida banco de dados, parser JSON e operações principais.

---

## 🚀 Como Rodar o Projeto

### **Pré-requisitos:**

* Windows 10/11
* GCC (MinGW ou MSYS2)
* Git (opcional)

### **Passo a Passo:**

1. **Clone o repositório:**

```bash
git clone https://github.com/Daniel16Bit/c-task-manager-api.git
cd c-task-manager-api/project
```

2. **Compile o projeto:**

```bash
gcc main.c database.c sqlite/sqlite3.c -o server.exe -lws2_32
```

3. **Execute o servidor:**

```bash
./server.exe
```

4. **Acesse no navegador:**

```
http://localhost:8080
```

---

## 📡 Endpoints da API

| Método | Endpoint   | Descrição      |
| ------ | ---------- | -------------- |
| GET    | /          | Página inicial |
| GET    | /tasks     | Lista tarefas  |
| GET    | /tasks/:id | Busca por ID   |
| POST   | /tasks     | Cria tarefa    |
| PUT    | /tasks/:id | Atualiza       |
| DELETE | /tasks/:id | Remove         |

---

## 🎓 O Que Aprendi

* Programação de sockets TCP/IP
* HTTP em baixo nível
* Gerenciamento manual de memória
* Integração C + SQLite
* Arquitetura REST

---

## 🧠 Decisões Técnicas

### **Por que C em vez de Node.js/Python?**
- Entender networking em baixo nível
- Dominar gerenciamento manual de memória
- Aprender como servidores funcionam "por baixo dos panos"

### **Por que SQLite?**
- Banco de dados leve e embutido
- Não requer servidor separado
- Ideal para projetos acadêmicos/portfólio

### **Por que construir HTTP manualmente?**
- Frameworks abstraem demais o funcionamento real
- Aprendizado profundo do protocolo HTTP
- Diferencial em entrevistas técnicas

## 📄 Licença

- Este projeto é de código aberto para fins educacionais.

## 👤 Autor

**Marcos Daniel**
- GitHub: [@Daniel16Bit](https://github.com/Daniel16Bit)
- LinkedIn: [Marcos Daniel](https://www.linkedin.com/in/marcos-daniel-245b94352/)
- Email: mdaniel.main@gmail.com

---

## 🙏 Agradecimentos

Projeto desenvolvido como parte dos meus estudos em Engenharia de Software, com foco em programação de sistemas e networking.


---
# 🗂️ C Task Manager API

## 🇺🇸 Description

> REST API for task management built **from scratch** in pure C, without frameworks.

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_%28programming_language%29)
[![Platform: Windows](https://img.shields.io/badge/Platform-Windows-green.svg)](https://www.microsoft.com/windows)
[![Database: SQLite](https://img.shields.io/badge/Database-SQLite-lightgrey.svg)](https://www.sqlite.org/)

---

## 📌 About the Project

This project implements a **complete REST API** in C, including:

* ✅ HTTP server manually built with **Winsock** (TCP/IP sockets)
* ✅ HTTP parser to process requests
* ✅ Routing system for multiple endpoints
* ✅ Data persistence using **SQLite**
* ✅ Full **CRUD** support (Create, Read, Update, Delete)

**Goal:** Strengthen knowledge of low-level programming, networking, and API architecture.

---

## 🛠️ Technologies Used

* **Language:** C (C11)
* **Networking:** Winsock2 (Windows Sockets API)
* **Database:** SQLite3
* **Protocol:** HTTP/1.1
* **Data Format:** JSON

---

## 📦 Installation and Dependencies

### **System Requirements:**

* **Operating System:** Windows 10/11 (64-bit)
* **Compiler:** GCC (via MinGW-w64 or MSYS2)
* **Libraries:** Winsock2 (included with Windows), SQLite3 (included in the project)

### **1. Installing the GCC Compiler**

#### **Option A: MSYS2 (Recommended)**

1. Download the installer: [https://www.msys2.org/](https://www.msys2.org/)
2. Install and run MSYS2 UCRT64
3. Update packages:

```bash
pacman -Syu
```

4. Install GCC:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

5. Add to PATH:

```
C:\msys64\ucrt64\bin
```

#### **Option B: MinGW-w64**

1. Download from: [https://www.mingw-w64.org/](https://www.mingw-w64.org/)
2. Install and add it to the system PATH

### **2. Dependencies Included in the Project**

The project **already includes all required dependencies**:

```
project/
└── sqlite/
    ├── sqlite3.h
    └── sqlite3.c
```

**SQLite is embedded in the project.** No separate installation is required.

### **3. System Libraries (Windows)**

* **Winsock2** (`ws2_32.dll`) – included with Windows
* Linked during compilation using `-lws2_32`

### **4. Verify Installation**

```bash
gcc --version
```

---

## 🏗️ Detailed Architecture

### **Complete File Structure:**

```
c-task-manager-api/
├── project/
│   ├── main.c
│   ├── server.h / server.c
│   ├── http.h / http.c
│   ├── json_parser.h / json_parser.c
│   ├── cli.h / cli.c
│   ├── tests.h / tests.c
│   ├── database.h / database.c
│   ├── sqlite/
│   │   ├── sqlite3.h
│   │   └── sqlite3.c
│   └── tasks.db
├── screenshots/
├── README.md
└── .gitignore
```

### **📂 Module Descriptions:**

#### **🎯 main.c – Entry Point**

Responsible for application initialization, interactive menu, and execution mode selection.

#### **🌐 server.c – Network Layer**

Manages TCP/IP sockets, accepts connections, and forwards requests to the HTTP module.

#### **📡 http.c – HTTP Processing**

Parses requests, routes endpoints, and builds HTTP/JSON responses.

#### **📋 json_parser.c – JSON Parser**

Extracts simple task data from raw JSON.

#### **💾 database.c – Persistence Layer**

Abstracts CRUD operations using embedded SQLite.

#### **🖥️ cli.c – CLI Interface**

Interactive command-line menu for task management.

#### **🧪 tests.c – Automated Tests**

Validates database operations, JSON parsing, and core functionalities.

---

## 🚀 How to Run the Project

### **Prerequisites:**

* Windows 10/11
* GCC (MinGW or MSYS2)
* Git (optional)

### **Step by Step:**

1. **Clone the repository:**

```bash
git clone https://github.com/Daniel16Bit/c-task-manager-api.git
cd c-task-manager-api/project
```

2. **Compile the project:**

```bash
gcc main.c database.c sqlite/sqlite3.c -o server.exe -lws2_32
```

3. **Run the server:**

```bash
./server.exe
```

4. **Access in the browser:**

```
http://localhost:8080
```

---

## 📡 API Endpoints

| Method | Endpoint   | Description    |
| ------ | ---------- | -------------- |
| GET    | /          | Home page      |
| GET    | /tasks     | List all tasks |
| GET    | /tasks/:id | Get task by ID |
| POST   | /tasks     | Create a task  |
| PUT    | /tasks/:id | Update a task  |
| DELETE | /tasks/:id | Delete a task  |

---

## 🎓 What I Learned

* TCP/IP socket programming
* Low-level HTTP handling
* Manual memory management
* C + SQLite integration
* REST architecture

---

## 🧠 Technical Decisions

### **Why C instead of Node.js/Python?**

* To understand low-level networking
* To master manual memory management
* To learn how servers work under the hood

### **Why SQLite?**

* Lightweight and embedded database
* No separate server required
* Ideal for academic and portfolio projects

### **Why build HTTP manually?**

* Frameworks hide how things actually work
* Deep understanding of the HTTP protocol
* Strong differentiator in technical interviews

---

## 📄 License

* This project is open source for educational purposes.

---

## 👤 Author

**Marcos Daniel**

* GitHub: [@Daniel16Bit](https://github.com/Daniel16Bit)
* LinkedIn: [Marcos Daniel](https://www.linkedin.com/in/marcos-daniel-245b94352/)
* Email: [mdaniel.main@gmail.com](mailto:mdaniel.main@gmail.com)

---

## 🙏 Acknowledgements

Project developed as part of my Software Engineering studies, with a focus on systems programming and networking.
