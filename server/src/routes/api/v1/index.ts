import { Router } from "express";

import lock from "./lock";

const router = Router();

router.get("/", (req, res) => {
    res.send("🔒 PlzLeaveMeAlone API v1 🔥");
});

router.use("/lock", lock);

export default router;