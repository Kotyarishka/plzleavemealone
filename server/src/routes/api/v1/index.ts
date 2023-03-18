import { Router } from "express";

import controlsRouter from "./controls";

const router = Router();

router.get("/", (req, res) => {
    res.send("Welcome to the API v1! :)");
});

router.use("/controls", controlsRouter);

export default router;