import express from "express";

import morgan from "morgan";
import helmet from "helmet";
import cors from "cors";

const app = express();

app.use(morgan("dev"));
app.use(helmet());
app.use(cors());

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

import routes from "./routes";

app.use(routes);

app.listen(3000, () => {
    console.log("Server is running on port 3000");
});