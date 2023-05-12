FROM node:18.3.0-alpine3.16

# Create app directory
WORKDIR /app

# Install app dependencies
COPY package.json ./
COPY pnpm-lock.yaml ./

RUN npm install -g pnpm

RUN pnpm install

# Bundle app source
COPY . .

EXPOSE 5173

CMD [ "pnpm", "run", "dev", "--host" ]
