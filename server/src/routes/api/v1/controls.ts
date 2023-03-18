import { Router } from "express";
import StateManager from "../../../classes/StateManager";
import RequestWithLockId from "../../../interfaces/request";
import getLockId from "../../../middlewares/getLockId";

const router = Router();

router.get("/", getLockId, (req: RequestWithLockId, res) => {
    const { lockId } = req;

    if (!lockId) {
        return res.status(400).send("Missing lockId");
    }

    if (StateManager.getState(lockId) == null) {
        return res.status(404).send("Lock not found");
    }

    res.send(StateManager.getState(lockId) ? "Locked" : "Unlocked");
});

router.get("/close", getLockId, (req: RequestWithLockId, res) => {
    const { lockId } = req;

    if (!lockId) {
        return res.status(400).send("Missing lockId");
    }

    if (StateManager.getState(lockId) == null) {
        return res.status(404).send("Lock not found");
    }

    StateManager.setState(lockId, false);

    res.send("OK");
});

router.get("/open", getLockId, (req: RequestWithLockId, res) => {
    const { lockId } = req;

    if (!lockId) {
        return res.status(400).send("Missing lockId");
    }

    if (StateManager.getState(lockId) == null) {
        return res.status(404).send("Lock not found");
    }

    StateManager.setState(lockId, true);

    res.send("OK");
});

export default router;