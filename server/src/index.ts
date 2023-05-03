import express from "express";

import morgan from "morgan";
import helmet from "helmet";
import cors from "cors";

import path from "path";

const app = express();

app.use(morgan("dev"));
app.use(helmet());
app.use(cors());

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.set('view engine', 'ejs');
app.set('views', path.join(__dirname, 'views'));
app.use(express.static(path.join(__dirname, 'public')));

import routes from "./routes";

app.use(routes);

app.listen(3000, () => {
    console.log("Server is running on port 3000");
});