# PokeZOO

PokeZOO is a fun project that aims to create a virtual zoo made up of animated Pokemon sprites. The entities to be drawn in the zoo will be fetched from the PokeAPI, and the project is built using a combination of WebAssembly, C++, SDL, React, and TypeScript.

### Demo

The project is available to view at the following URL: https://pokezoo.fly.dev/

Hosted by [Fly.io](https://fly.io/)
![PokeZOO Demo](assets/demo/v1.0.0.gif)

## Installation

To run this project on your local machine, you will need to follow the steps below:

1. Clone the repository to your local machine using the command below:

```bash
git clone https://github.com/adia-dev/PokeZOO.git
```

2. Navigate to the root directory of the project and run the following command to install the dependencies:

```bash
pnpm install
```

If you don't have `pnpm` installed, you can install it by running:

```bash
npm install -g pnpm
```

3. Build the C++ WebAssembly module using the following command:

```bash
pnpm run build:wasm
```

4. Build the frontend using the following command:

```bash
pnpm run build:frontend
```

5. Start the API using the following command:

```bash
pnpm run start:api
```

6. Start the frontend using the following command:
```bash
pnpm run start:frontend
```


7. Open a browser and go to `http://localhost:3000/` to see the PokeZOO application.

Alternatively, you can start both the API and the frontend using the following command:



## Usage

Upon starting the application, you will be presented with a virtual zoo filled with animated Pokemon sprites. You can interact with the sprites by clicking on them, which will display their details, including their name, type, abilities, and stats. You can also use the search bar at the top of the page to search for specific Pokemon entities.

## Contributing

We welcome contributions to the PokeZOO project. If you would like to contribute, please follow the steps below:

1. Fork the repository to your own GitHub account.
2. Create a new branch with a descriptive name for your contribution.
3. Make your changes to the code, and add any necessary documentation.
4. Submit a pull request, and our team will review your changes and merge them if they meet our standards.

## Credits

The PokeZOO project was built using a combination of open-source technologies, including:

- WebAssembly
- C++
- SDL
- React
- TypeScript
- PokeAPI

## License

The PokeZOO project is licensed under the MIT license, which means that you are free to use, modify, and distribute the code as you wish. However, we ask that you credit the original developers of the project if you decide to use or modify it.
