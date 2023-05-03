import { Router } from "express";
import LocksManager from "../classes/LocksManager";
import { RequestWithAllLocks } from "../interfaces/request";
import getAllLocks from "../middlewares/getAllLocks";

import api from "./api";

const router = Router();

router.use("/api", api);

router.get("/", getAllLocks, (req: RequestWithAllLocks, res) => {
    res.render("index", { locks: req.locks });
});

export default router;